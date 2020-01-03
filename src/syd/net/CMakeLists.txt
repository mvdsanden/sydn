cmake_minimum_required(VERSION 3.7)

syd_add_library(
  NAME sydnet
  COMPONENTS
  address
  ipv4_address
  unix_address
  connected_socket
  listen_socket
  DEPENDENCIES
  sydmem
  GSL)
