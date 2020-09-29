#pragma once
#ifndef STARDUST_H
#define STARDUST_H

#include "application/Application.h"
#include "assets/AssetManager.h"

#include "audio/Listener.h"
#include "audio/sounds/SoundBase.h"
#include "audio/sounds/SoundEnums.h"
#include "audio/sounds/Sounds.h"
#include "audio/SoundSource.h"
#include "audio/SoundSystem.h"
#include "audio/volume/VolumeManager.h"

#include "config/Config.h"
#include "debug/logging/Log.h"
#include "debug/message_box/MessageBox.h"

#include "graphics/Colour.h"
#include "graphics/display/Display.h"
#include "graphics/renderer/Renderer.h"
#include "graphics/texture/Texture.h"
#include "graphics/window/Window.h"

#include "input/Input.h"
#include "input/KeyCode.h"
#include "locale/Locale.h"

#include "math/Math.h"
#include "rect/Rect.h"

#include "scene/Scene.h"
#include "scene/SceneManager.h"
#include "scene/components/Components.h"
#include "scene/entity/Entity.h"

#include "text/clipboard/Clipboard.h"
#include "text/text_input/TextInput.h"

#include "utility/cpu/CPU.h"
#include "utility/enums/Status.h"
#include "utility/interfaces/INoncopyable.h"
#include "utility/interfaces/INonmovable.h"
#include "utility/random/Random.h"

#include "vfs/VFS.h"

#endif