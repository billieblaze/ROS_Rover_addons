
(cl:in-package :asdf)

(defsystem "rover-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :sensor_msgs-msg
)
  :components ((:file "_package")
    (:file "RangeLocation" :depends-on ("_package_RangeLocation"))
    (:file "_package_RangeLocation" :depends-on ("_package"))
  ))