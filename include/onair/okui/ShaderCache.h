#pragma once

#include "onair/okui/config.h"

#include "scraps/Cache.h"

#include "onair/okui/Shader.h"

namespace onair {
namespace okui {

using ShaderCacheEntry = std::unique_ptr<Shader>;
using ShaderCache = scraps::Cache<ShaderCacheEntry>;

} } //namespace onair::okui
