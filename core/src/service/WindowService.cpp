//
// Created by arunc on 01/07/2025.
//
#include "service/WindowService.hpp"

WindowService* WindowService::instance = nullptr;
std::mutex WindowService::mtx;