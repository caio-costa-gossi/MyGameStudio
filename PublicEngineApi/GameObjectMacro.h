#pragma once

#define REGISTER_GAME_OBJECT(ClassName)                                     \
namespace {                                                                 \
        class ObjectFactory_##ClassName : public GameObjectFactory {        \
		public:                                                             \
			GameObject* Get() { return new ClassName(); }                   \
		};                                                                  \
        struct AutoRegister_##ClassName {                                   \
            AutoRegister_##ClassName() {                                    \
                GameObjectManager::Get().AddFactory(                        \
                    new ObjectFactory_##ClassName());                       \
            }                                                               \
        } _autoRegisterInstance_##ClassName;                                \
    }