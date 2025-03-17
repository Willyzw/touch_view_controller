#ifndef TOUCH_VIEW_CONTROLLER_H
#define TOUCH_VIEW_CONTROLLER_H

#include <rviz/view_controller.h>
#include <rviz/default_plugin/view_controllers/third_person_follower_view_controller.h>
#include <QGestureEvent>
#include <QPinchGesture>

namespace touch_view_controller
{

class TouchViewController : public rviz::ThirdPersonFollowerViewController
{
  Q_OBJECT

public:
  TouchViewController();
  virtual ~TouchViewController();

  /** @brief Do subclass-specific initialization.  Called by
   * ViewController::initialize after context_ and camera_ are set.
   *
   * This version sets up the focus point and distance. */
  virtual void onInitialize() override;

  /** @brief Called when this view controller becomes the active view controller. */
  virtual void mimic(rviz::ViewController* source_view) override;

  /** @brief Update camera position and orientation to keep an eye on the tracked object. */
  virtual void update(float dt, float ros_dt) override;

  /** @brief Handle any gestures for touchscreen interaction. */
  bool event(QEvent* event) override;
  
  /** @brief Event filter to capture touch events */
  bool eventFilter(QObject* obj, QEvent* event) override;

protected:
  bool gestureEvent(QGestureEvent* event);
  void pinchTriggered(QPinchGesture* gesture);

  // Tracking pinch gesture state
  float current_scale_factor_;
  float last_scale_factor_;
  bool pinch_active_;
};

} // namespace touch_view_controller

#endif // TOUCH_VIEW_CONTROLLER_H