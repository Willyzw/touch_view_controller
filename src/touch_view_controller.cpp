#include "touch_view_controller/touch_view_controller.h"
#include <pluginlib/class_list_macros.h>
#include <rviz/display_context.h>
#include <rviz/render_panel.h>
#include <rviz/view_manager.h>
#include <rviz/viewport_mouse_event.h>
#include <rviz/properties/float_property.h>
#include <rviz/properties/vector_property.h>
#include <rviz/properties/bool_property.h>
#include <OgreCamera.h>
#include <QWidget>
#include <QTouchEvent>

namespace touch_view_controller
{

TouchViewController::TouchViewController()
  : current_scale_factor_(1.0f)
  , last_scale_factor_(1.0f)
  , pinch_active_(false)
{
}

TouchViewController::~TouchViewController()
{
}

void TouchViewController::onInitialize()
{
  // Call parent's initialization
  ThirdPersonFollowerViewController::onInitialize();
  
  // Enable gesture recognition for the render panel
  rviz::RenderPanel* render_panel = context_->getViewManager()->getRenderPanel();
  if (render_panel)
  {
    // Make the panel accept touch events directly
    render_panel->setAttribute(Qt::WA_AcceptTouchEvents);
    
    // Enable specific gesture types
    render_panel->grabGesture(Qt::PinchGesture);
    render_panel->grabGesture(Qt::SwipeGesture);
    render_panel->grabGesture(Qt::PanGesture);
    
    // Install event filter to capture all events
    render_panel->installEventFilter(this);
  }
}

void TouchViewController::mimic(rviz::ViewController* source_view)
{
  ThirdPersonFollowerViewController::mimic(source_view);
}

void TouchViewController::update(float dt, float ros_dt)
{
  ThirdPersonFollowerViewController::update(dt, ros_dt);
}

// Event filter to capture touch events before they're processed as mouse events
bool TouchViewController::eventFilter(QObject* obj, QEvent* event)
{
  // Check for touch events
  if (event->type() == QEvent::TouchBegin ||
      event->type() == QEvent::TouchUpdate ||
      event->type() == QEvent::TouchEnd) 
  {
    QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
    
    // Handle two-finger pinch manually
    const QList<QTouchEvent::TouchPoint>& touchPoints = touchEvent->touchPoints();
    if (touchPoints.count() == 2) {
      // Get the two touch points
      const QTouchEvent::TouchPoint& touchPoint0 = touchPoints.at(0);
      const QTouchEvent::TouchPoint& touchPoint1 = touchPoints.at(1);
      
      // Calculate the current distance between touch points
      QLineF line(touchPoint0.pos(), touchPoint1.pos());
      qreal currentLength = line.length();
      
      // Calculate the previous distance between touch points
      QLineF prevLine(touchPoint0.lastPos(), touchPoint1.lastPos());
      qreal previousLength = prevLine.length();
      
      if (previousLength > 0) {
        // Calculate the zoom factor
        qreal zoomFactor = currentLength / previousLength;
        
        // Apply zoom by changing the distance property with a slower factor
        if (zoomFactor > 1.0) {
          // Zooming in - decrease distance more gradually (0.98 instead of 0.95)
          distance_property_->setFloat(distance_property_->getFloat() * 0.99f);
        } else if (zoomFactor < 1.0) {
          // Zooming out - increase distance more gradually (1.02 instead of 1.05)
          distance_property_->setFloat(distance_property_->getFloat() * 1.01f);
        }
      }
      
      // Don't pass the event up
      return true;
    }
  }
  
  // Process gesture events
  if (event->type() == QEvent::Gesture) {
    QGestureEvent* gestureEvent = static_cast<QGestureEvent*>(event);
    
    if (QPinchGesture* pinchGesture = static_cast<QPinchGesture*>(
          gestureEvent->gesture(Qt::PinchGesture))) {
      
      // Process the pinch
      if (pinchGesture->changeFlags() & QPinchGesture::ScaleFactorChanged) {
        qreal scaleFactor = pinchGesture->scaleFactor();
        
        // Apply zoom based on scale factor with slower zoom speed
        if (scaleFactor > 1.0) {
          // Zooming in - decrease distance more gradually
          distance_property_->setFloat(distance_property_->getFloat() * 0.98f);
        } else if (scaleFactor < 1.0) {
          // Zooming out - increase distance more gradually
          distance_property_->setFloat(distance_property_->getFloat() * 1.02f);
        }
      }
      
      // Accept the gesture event
      gestureEvent->accept(pinchGesture);
      return true;
    }
  }
  
  // For other event types, pass to the parent class
  return ThirdPersonFollowerViewController::eventFilter(obj, event);
}

bool TouchViewController::event(QEvent* event)
{
  if (event->type() == QEvent::Gesture) {
    gestureEvent(static_cast<QGestureEvent*>(event));
    return true;
  }
  
  return ThirdPersonFollowerViewController::event(event);
}

bool TouchViewController::gestureEvent(QGestureEvent* event)
{
  if (QPinchGesture* pinch = static_cast<QPinchGesture*>(event->gesture(Qt::PinchGesture))) {
    pinchTriggered(pinch);
    event->accept();
    return true;
  }
  
  return false;
}

void TouchViewController::pinchTriggered(QPinchGesture* gesture)
{
  QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
  
  if (changeFlags & QPinchGesture::ScaleFactorChanged) {
    current_scale_factor_ = gesture->totalScaleFactor();
    
    // Calculate zoom direction with slower zoom speed
    if (current_scale_factor_ > last_scale_factor_) {
      // Zoom in - decrease distance with a gentler factor
      float newDistance = distance_property_->getFloat() * 0.99f;
      distance_property_->setFloat(newDistance);
    }
    else if (current_scale_factor_ < last_scale_factor_) {
      // Zoom out - increase distance with a gentler factor
      float newDistance = distance_property_->getFloat() * 1.01f;
      distance_property_->setFloat(newDistance);
    }
    
    last_scale_factor_ = current_scale_factor_;
  }
  
  if (gesture->state() == Qt::GestureStarted) {
    pinch_active_ = true;
    last_scale_factor_ = 1.0f;
    current_scale_factor_ = 1.0f;
  }
  else if (gesture->state() == Qt::GestureFinished) {
    pinch_active_ = false;
  }
}

} // namespace touch_view_controller

// Tell pluginlib about this class
PLUGINLIB_EXPORT_CLASS(touch_view_controller::TouchViewController, rviz::ViewController)