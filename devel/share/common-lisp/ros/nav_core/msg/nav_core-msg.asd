
(cl:in-package :asdf)

(defsystem "nav_core-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "pos_vel_att_msg" :depends-on ("_package_pos_vel_att_msg"))
    (:file "_package_pos_vel_att_msg" :depends-on ("_package"))
    (:file "remote_control_msg" :depends-on ("_package_remote_control_msg"))
    (:file "_package_remote_control_msg" :depends-on ("_package"))
    (:file "sbus_channels_msg" :depends-on ("_package_sbus_channels_msg"))
    (:file "_package_sbus_channels_msg" :depends-on ("_package"))
  ))