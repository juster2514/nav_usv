;; Auto-generated. Do not edit!


(when (boundp 'nav_core::sbus_channels_msg)
  (if (not (find-package "NAV_CORE"))
    (make-package "NAV_CORE"))
  (shadow 'sbus_channels_msg (find-package "NAV_CORE")))
(unless (find-package "NAV_CORE::SBUS_CHANNELS_MSG")
  (make-package "NAV_CORE::SBUS_CHANNELS_MSG"))

(in-package "ROS")
;;//! \htmlinclude sbus_channels_msg.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass nav_core::sbus_channels_msg
  :super ros::object
  :slots (_header _channels_value ))

(defmethod nav_core::sbus_channels_msg
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:channels_value __channels_value) (make-array 16 :initial-element 0 :element-type :integer))
    )
   (send-super :init)
   (setq _header __header)
   (setq _channels_value __channels_value)
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:channels_value
   (&optional __channels_value)
   (if __channels_value (setq _channels_value __channels_value)) _channels_value)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; uint16[16] _channels_value
    (* 2    16)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; uint16[16] _channels_value
     (dotimes (i 16)
       (write-word (elt _channels_value i) s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; uint16[16] _channels_value
   (dotimes (i (length _channels_value))
     (setf (elt _channels_value i) (sys::peek buf ptr- :short)) (incf ptr- 2)
     )
   ;;
   self)
  )

(setf (get nav_core::sbus_channels_msg :md5sum-) "2e090f1a68ca526969f62f98dbfe335b")
(setf (get nav_core::sbus_channels_msg :datatype-) "nav_core/sbus_channels_msg")
(setf (get nav_core::sbus_channels_msg :definition-)
      "std_msgs/Header header
uint16[16] channels_value
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

")



(provide :nav_core/sbus_channels_msg "2e090f1a68ca526969f62f98dbfe335b")


