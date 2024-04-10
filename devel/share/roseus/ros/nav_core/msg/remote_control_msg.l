;; Auto-generated. Do not edit!


(when (boundp 'nav_core::remote_control_msg)
  (if (not (find-package "NAV_CORE"))
    (make-package "NAV_CORE"))
  (shadow 'remote_control_msg (find-package "NAV_CORE")))
(unless (find-package "NAV_CORE::REMOTE_CONTROL_MSG")
  (make-package "NAV_CORE::REMOTE_CONTROL_MSG"))

(in-package "ROS")
;;//! \htmlinclude remote_control_msg.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass nav_core::remote_control_msg
  :super ros::object
  :slots (_header _ch_1 _ch_2 _key_value ))

(defmethod nav_core::remote_control_msg
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:ch_1 __ch_1) 0)
    ((:ch_2 __ch_2) 0)
    ((:key_value __key_value) 0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _ch_1 (round __ch_1))
   (setq _ch_2 (round __ch_2))
   (setq _key_value (round __key_value))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:ch_1
   (&optional __ch_1)
   (if __ch_1 (setq _ch_1 __ch_1)) _ch_1)
  (:ch_2
   (&optional __ch_2)
   (if __ch_2 (setq _ch_2 __ch_2)) _ch_2)
  (:key_value
   (&optional __key_value)
   (if __key_value (setq _key_value __key_value)) _key_value)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; int16 _ch_1
    2
    ;; int16 _ch_2
    2
    ;; int16 _key_value
    2
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; int16 _ch_1
       (write-word _ch_1 s)
     ;; int16 _ch_2
       (write-word _ch_2 s)
     ;; int16 _key_value
       (write-word _key_value s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; int16 _ch_1
     (setq _ch_1 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _ch_2
     (setq _ch_2 (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _key_value
     (setq _key_value (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;;
   self)
  )

(setf (get nav_core::remote_control_msg :md5sum-) "8c560d5e5383d834df3135169b349720")
(setf (get nav_core::remote_control_msg :datatype-) "nav_core/remote_control_msg")
(setf (get nav_core::remote_control_msg :definition-)
      "std_msgs/Header header
int16 ch_1
int16 ch_2
int16 key_value
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



(provide :nav_core/remote_control_msg "8c560d5e5383d834df3135169b349720")


