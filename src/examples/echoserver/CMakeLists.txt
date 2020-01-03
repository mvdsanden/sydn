cmake_minimum_required(VERSION 3.7)

syd_add_task(
  NAME echoserver
  COMPONENTS
  echoserver
  DEPENDENCIES
  sydnet
  pthread)
