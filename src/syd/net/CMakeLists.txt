cmake_minimum_required(VERSION 3.7)

syd_add_library(
  NAME sydnet
  COMPONENTS
  address
  ipv4_address
  unix_address
  socket_buffer
  posix_socket_buffer
  connected_socket
  connected_reader
  connected_writer
  listen_socket
  DEPENDENCIES
  sydmem
  GSL)
