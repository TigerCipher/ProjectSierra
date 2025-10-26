// **************************************************************************** //
// Copyright 2025 Matt Rogers
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// **************************************************************************** //



#include "GLDebug.h"

#include "Common.h"
#include <array>

#include <glad/glad.h>

namespace stick
{

namespace
{

// The prototype must match the GL callback signature. APIENTRY is provided by glad.
void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id,
                                     GLenum severity, GLsizei length,
                                     const GLchar* message, const void* userParam)
{
    // Optional: ignore noisy/per-driver message IDs
    // if (id == 131185) return; // example: NVIDIA spurious performance warning

    auto srcStr = [source]() {
        switch (source) {
            case GL_DEBUG_SOURCE_API: return "API";
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window";
            case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader";
            case GL_DEBUG_SOURCE_THIRD_PARTY: return "3rdParty";
            case GL_DEBUG_SOURCE_APPLICATION: return "App";
            case GL_DEBUG_SOURCE_OTHER: default: return "Other";
        }
    }();

    auto typeStr = [type]() {
        switch (type) {
            case GL_DEBUG_TYPE_ERROR: return "Error";
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "Deprecated";
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "Undefined";
            case GL_DEBUG_TYPE_PORTABILITY: return "Portability";
            case GL_DEBUG_TYPE_PERFORMANCE: return "Performance";
            case GL_DEBUG_TYPE_MARKER: return "Marker";
            case GL_DEBUG_TYPE_PUSH_GROUP: return "PushGroup";
            case GL_DEBUG_TYPE_POP_GROUP: return "PopGroup";
            case GL_DEBUG_TYPE_OTHER: default: return "Other";
        }
    }();

    // Map GL severity to your LOG_* macros (fmt-style assumed)
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            LOG_ERROR("[GL][{}][{}] {} (id={})", srcStr, typeStr, message ? message : "<null>", id);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            LOG_WARN("[GL][{}][{}] {} (id={})", srcStr, typeStr, message ? message : "<null>", id);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            LOG_INFO("[GL][{}][{}] {} (id={})", srcStr, typeStr, message ? message : "<null>", id);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
        default:
            LOG_DEBUG("[GL][{}][{}] {} (id={})", srcStr, typeStr, message ? message : "<null>", id);
            break;
    }
}

}

void EnableGLDebug(DebugSeverityLevel minLevel, bool synchronous)
{
    glEnable(GL_DEBUG_OUTPUT);
    if (synchronous)
    {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    glDebugMessageCallback(GLDebugCallback, nullptr);

    // Control which messages to enable based on severity
    std::array<GLenum, 4> severities = {
        GL_DEBUG_SEVERITY_NOTIFICATION,
        GL_DEBUG_SEVERITY_LOW,
        GL_DEBUG_SEVERITY_MEDIUM,
        GL_DEBUG_SEVERITY_HIGH
    };

    for (size_t i = 0; i < severities.size(); ++i)
    {
        if (static_cast<u8>(minLevel) <= i)
        {
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, severities[i], 0, nullptr, GL_TRUE);
        }
        else
        {
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, severities[i], 0, nullptr, GL_FALSE);
        }
    }

    LOG_INFO("OpenGL Debugging enabled with minimum severity level: {}", static_cast<u8>(minLevel));
}

} // namespace stick