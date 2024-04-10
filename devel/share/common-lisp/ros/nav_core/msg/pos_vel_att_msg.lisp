; Auto-generated. Do not edit!


(cl:in-package nav_core-msg)


;//! \htmlinclude pos_vel_att_msg.msg.html

(cl:defclass <pos_vel_att_msg> (roslisp-msg-protocol:ros-message)
  ((header
    :reader header
    :initarg :header
    :type std_msgs-msg:Header
    :initform (cl:make-instance 'std_msgs-msg:Header))
   (pos
    :reader pos
    :initarg :pos
    :type geometry_msgs-msg:PoseStamped
    :initform (cl:make-instance 'geometry_msgs-msg:PoseStamped))
   (vel
    :reader vel
    :initarg :vel
    :type geometry_msgs-msg:Vector3
    :initform (cl:make-instance 'geometry_msgs-msg:Vector3)))
)

(cl:defclass pos_vel_att_msg (<pos_vel_att_msg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <pos_vel_att_msg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'pos_vel_att_msg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name nav_core-msg:<pos_vel_att_msg> is deprecated: use nav_core-msg:pos_vel_att_msg instead.")))

(cl:ensure-generic-function 'header-val :lambda-list '(m))
(cl:defmethod header-val ((m <pos_vel_att_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:header-val is deprecated.  Use nav_core-msg:header instead.")
  (header m))

(cl:ensure-generic-function 'pos-val :lambda-list '(m))
(cl:defmethod pos-val ((m <pos_vel_att_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:pos-val is deprecated.  Use nav_core-msg:pos instead.")
  (pos m))

(cl:ensure-generic-function 'vel-val :lambda-list '(m))
(cl:defmethod vel-val ((m <pos_vel_att_msg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader nav_core-msg:vel-val is deprecated.  Use nav_core-msg:vel instead.")
  (vel m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <pos_vel_att_msg>) ostream)
  "Serializes a message object of type '<pos_vel_att_msg>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'header) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'pos) ostream)
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'vel) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <pos_vel_att_msg>) istream)
  "Deserializes a message object of type '<pos_vel_att_msg>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'header) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'pos) istream)
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'vel) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<pos_vel_att_msg>)))
  "Returns string type for a message object of type '<pos_vel_att_msg>"
  "nav_core/pos_vel_att_msg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'pos_vel_att_msg)))
  "Returns string type for a message object of type 'pos_vel_att_msg"
  "nav_core/pos_vel_att_msg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<pos_vel_att_msg>)))
  "Returns md5sum for a message object of type '<pos_vel_att_msg>"
  "a67d873ea38c38e5846a867f7cf7cb1b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'pos_vel_att_msg)))
  "Returns md5sum for a message object of type 'pos_vel_att_msg"
  "a67d873ea38c38e5846a867f7cf7cb1b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<pos_vel_att_msg>)))
  "Returns full string definition for message of type '<pos_vel_att_msg>"
  (cl:format cl:nil "std_msgs/Header header~%geometry_msgs/PoseStamped pos~%geometry_msgs/Vector3 vel~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PoseStamped~%# A Pose with reference coordinate frame and timestamp~%Header header~%Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: geometry_msgs/Vector3~%# This represents a vector in free space. ~%# It is only meant to represent a direction. Therefore, it does not~%# make sense to apply a translation to it (e.g., when applying a ~%# generic rigid transformation to a Vector3, tf2 will only apply the~%# rotation). If you want your data to be translatable too, use the~%# geometry_msgs/Point message instead.~%~%float64 x~%float64 y~%float64 z~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'pos_vel_att_msg)))
  "Returns full string definition for message of type 'pos_vel_att_msg"
  (cl:format cl:nil "std_msgs/Header header~%geometry_msgs/PoseStamped pos~%geometry_msgs/Vector3 vel~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PoseStamped~%# A Pose with reference coordinate frame and timestamp~%Header header~%Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%================================================================================~%MSG: geometry_msgs/Vector3~%# This represents a vector in free space. ~%# It is only meant to represent a direction. Therefore, it does not~%# make sense to apply a translation to it (e.g., when applying a ~%# generic rigid transformation to a Vector3, tf2 will only apply the~%# rotation). If you want your data to be translatable too, use the~%# geometry_msgs/Point message instead.~%~%float64 x~%float64 y~%float64 z~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <pos_vel_att_msg>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'header))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'pos))
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'vel))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <pos_vel_att_msg>))
  "Converts a ROS message object to a list"
  (cl:list 'pos_vel_att_msg
    (cl:cons ':header (header msg))
    (cl:cons ':pos (pos msg))
    (cl:cons ':vel (vel msg))
))
