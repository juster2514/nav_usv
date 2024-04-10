# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "nav_core: 3 messages, 0 services")

set(MSG_I_FLAGS "-Inav_core:/home/ljj/Nav_USV_ws/src/nav_core/msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/noetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(nav_core_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg" NAME_WE)
add_custom_target(_nav_core_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "nav_core" "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg" NAME_WE)
add_custom_target(_nav_core_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "nav_core" "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg" NAME_WE)
add_custom_target(_nav_core_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "nav_core" "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg" "geometry_msgs/Point:geometry_msgs/Vector3:geometry_msgs/Pose:std_msgs/Header:geometry_msgs/PoseStamped:geometry_msgs/Quaternion"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/nav_core
)
_generate_msg_cpp(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/nav_core
)
_generate_msg_cpp(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/nav_core
)

### Generating Services

### Generating Module File
_generate_module_cpp(nav_core
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/nav_core
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(nav_core_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(nav_core_generate_messages nav_core_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_cpp _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_cpp _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_cpp _nav_core_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(nav_core_gencpp)
add_dependencies(nav_core_gencpp nav_core_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS nav_core_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/nav_core
)
_generate_msg_eus(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/nav_core
)
_generate_msg_eus(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/nav_core
)

### Generating Services

### Generating Module File
_generate_module_eus(nav_core
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/nav_core
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(nav_core_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(nav_core_generate_messages nav_core_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_eus _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_eus _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_eus _nav_core_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(nav_core_geneus)
add_dependencies(nav_core_geneus nav_core_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS nav_core_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/nav_core
)
_generate_msg_lisp(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/nav_core
)
_generate_msg_lisp(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/nav_core
)

### Generating Services

### Generating Module File
_generate_module_lisp(nav_core
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/nav_core
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(nav_core_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(nav_core_generate_messages nav_core_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_lisp _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_lisp _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_lisp _nav_core_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(nav_core_genlisp)
add_dependencies(nav_core_genlisp nav_core_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS nav_core_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/nav_core
)
_generate_msg_nodejs(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/nav_core
)
_generate_msg_nodejs(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/nav_core
)

### Generating Services

### Generating Module File
_generate_module_nodejs(nav_core
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/nav_core
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(nav_core_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(nav_core_generate_messages nav_core_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_nodejs _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_nodejs _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_nodejs _nav_core_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(nav_core_gennodejs)
add_dependencies(nav_core_gennodejs nav_core_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS nav_core_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/nav_core
)
_generate_msg_py(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/nav_core
)
_generate_msg_py(nav_core
  "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/nav_core
)

### Generating Services

### Generating Module File
_generate_module_py(nav_core
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/nav_core
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(nav_core_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(nav_core_generate_messages nav_core_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/sbus_channels_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_py _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/remote_control_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_py _nav_core_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/ljj/Nav_USV_ws/src/nav_core/msg/pos_vel_att_msg.msg" NAME_WE)
add_dependencies(nav_core_generate_messages_py _nav_core_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(nav_core_genpy)
add_dependencies(nav_core_genpy nav_core_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS nav_core_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/nav_core)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/nav_core
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(nav_core_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(nav_core_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/nav_core)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/nav_core
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(nav_core_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(nav_core_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/nav_core)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/nav_core
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(nav_core_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(nav_core_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/nav_core)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/nav_core
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(nav_core_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(nav_core_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/nav_core)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/nav_core\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/nav_core
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(nav_core_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(nav_core_generate_messages_py geometry_msgs_generate_messages_py)
endif()
