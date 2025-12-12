#ifndef FIVM_APPLICATION_BASE_HPP
#define FIVM_APPLICATION_BASE_HPP

#include <memory>

#include "application.hpp"

std::unique_ptr<Application> createApplication() {
  return std::make_unique<Application>();
}

#endif /* FIVM_APPLICATION_BASE_HPP */