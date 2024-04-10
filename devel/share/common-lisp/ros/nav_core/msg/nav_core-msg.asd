
(cl:in-package :asdf)

(defsystem "nav_core-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "remote_control_msg" :depends-on ("_package_remote_control_msg"))
    (:file "_package_remote_control_msg" :depends-on ("_package"))
    (:file "sbus_channels_msg" :depends-on ("_package_sbus_channels_msg"))
    (:file "_package_sbus_channels_msg" :depends-on ("_package"))
  ))