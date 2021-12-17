load("@rules_cc//cc:defs.bzl", "cc_proto_library") 
load("@rules_java//java:defs.bzl", "java_proto_library") 
load("@rules_proto//proto:defs.bzl", "proto_library") 

java_proto_library( 
    name = "OneHotEncoderJava", 
    deps = [":OneHotEncoder"], 
)

cc_proto_library( 
    name = "OneHotEncoderCC", 
    deps = [":OneHotEncoder"], 
)

proto_library( 
    name = "OneHotEncoder", 
    srcs = ["OneHotEncoder.proto"], 
)

package(default_visibility = ["//visibility:public"])