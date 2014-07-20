set( CMAKE_INCLUDE_CURRENT_DIR ON )

set( GME_DIR "${PROJECT_SOURCE_DIR}/extlibs/gme-nes" )
set( GME_SOURCE_FILES
    ${GME_DIR}/Blip_Buffer.cpp
    ${GME_DIR}/Classic_Emu.cpp
    ${GME_DIR}/Data_Reader.cpp
    ${GME_DIR}/Effects_Buffer.cpp
    ${GME_DIR}/gme.cpp
    ${GME_DIR}/Gme_File.cpp
    ${GME_DIR}/M3u_Playlist.cpp
    ${GME_DIR}/Multi_Buffer.cpp
    ${GME_DIR}/Music_Emu.cpp
    ${GME_DIR}/Nes_Apu.cpp
    ${GME_DIR}/Nes_Cpu.cpp
    ${GME_DIR}/Nes_Fme7_Apu.cpp
    ${GME_DIR}/Nes_Namco_Apu.cpp
    ${GME_DIR}/Nes_Oscs.cpp
    ${GME_DIR}/Nes_Vrc6_Apu.cpp
    ${GME_DIR}/Nsfe_Emu.cpp
    ${GME_DIR}/Nsf_Emu.cpp
)

set( JSONCPP_DIR "${PROJECT_SOURCE_DIR}/extlibs/jsoncpp" )
set( JSONCPP_SOURCE_FILES
    ${JSONCPP_DIR}/src/json_reader.cpp
    ${JSONCPP_DIR}/src/json_value.cpp
    ${JSONCPP_DIR}/src/json_writer.cpp
)

set( PHYSFS_STREAM_DIR "${PROJECT_SOURCE_DIR}/extlibs/physfs-stream" )
set( PHYSFS_STREAM_SOURCE_FILES
    ${PHYSFS_STREAM_DIR}/physfs/ifile_stream.cpp
    ${PHYSFS_STREAM_DIR}/physfs/ifile_streambuf.cpp
    ${PHYSFS_STREAM_DIR}/physfs/ofile_stream.cpp
    ${PHYSFS_STREAM_DIR}/physfs/ofile_streambuf.cpp
    ${PHYSFS_STREAM_DIR}/physfs/physfs_file_system.cpp
)

set( SQUIRREL_DIR "${PROJECT_SOURCE_DIR}/extlibs/squirrel" )
set( SQUIRREL_FILES
    # Squirrel core
    ${SQUIRREL_DIR}/squirrel/sqapi.cpp
    ${SQUIRREL_DIR}/squirrel/sqbaselib.cpp
    ${SQUIRREL_DIR}/squirrel/sqclass.cpp
    ${SQUIRREL_DIR}/squirrel/sqcompiler.cpp
    ${SQUIRREL_DIR}/squirrel/sqdebug.cpp
    ${SQUIRREL_DIR}/squirrel/sqfuncstate.cpp
    ${SQUIRREL_DIR}/squirrel/sqlexer.cpp
    ${SQUIRREL_DIR}/squirrel/sqmem.cpp
    ${SQUIRREL_DIR}/squirrel/sqobject.cpp
    ${SQUIRREL_DIR}/squirrel/sqstate.cpp
    ${SQUIRREL_DIR}/squirrel/sqtable.cpp
    ${SQUIRREL_DIR}/squirrel/sqvm.cpp

    # Squirrel standard library
    ${SQUIRREL_DIR}/sqstdlib/sqstdaux.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdblob.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdio.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdmath.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdrex.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdstream.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdstring.cpp
    ${SQUIRREL_DIR}/sqstdlib/sqstdsystem.cpp
)

set( SQRAT_DIR "${PROJECT_SOURCE_DIR}/extlibs/sqrat" )

set( GUICHAN_DIR "${PROJECT_SOURCE_DIR}/extlibs/guichan" )
set( GUICHAN_SOURCE_FILES
    # Guichan core
    ${GUICHAN_DIR}/src/actionevent.cpp
    ${GUICHAN_DIR}/src/genericinput.cpp
    ${GUICHAN_DIR}/src/keyinput.cpp
    ${GUICHAN_DIR}/src/cliprectangle.cpp
    ${GUICHAN_DIR}/src/graphics.cpp
    ${GUICHAN_DIR}/src/mouseevent.cpp
    ${GUICHAN_DIR}/src/color.cpp
    ${GUICHAN_DIR}/src/gui.cpp
    ${GUICHAN_DIR}/src/mouseinput.cpp
    ${GUICHAN_DIR}/src/containerevent.cpp
    ${GUICHAN_DIR}/src/guichan.cpp
    ${GUICHAN_DIR}/src/rectangle.cpp
    ${GUICHAN_DIR}/src/defaultfont.cpp
    ${GUICHAN_DIR}/src/image.cpp
    ${GUICHAN_DIR}/src/selectionevent.cpp
    ${GUICHAN_DIR}/src/event.cpp
    ${GUICHAN_DIR}/src/imagefont.cpp
    ${GUICHAN_DIR}/src/text.cpp
    ${GUICHAN_DIR}/src/exception.cpp
    ${GUICHAN_DIR}/src/inputevent.cpp
    ${GUICHAN_DIR}/src/widget.cpp
    ${GUICHAN_DIR}/src/focushandler.cpp
    ${GUICHAN_DIR}/src/key.cpp
    ${GUICHAN_DIR}/src/font.cpp
    ${GUICHAN_DIR}/src/keyevent.cpp

    # Guichan widgets
    ${GUICHAN_DIR}/src/widgets/button.cpp
    ${GUICHAN_DIR}/src/widgets/checkbox.cpp
    ${GUICHAN_DIR}/src/widgets/container.cpp
    ${GUICHAN_DIR}/src/widgets/dropdown.cpp
    ${GUICHAN_DIR}/src/widgets/icon.cpp
    ${GUICHAN_DIR}/src/widgets/imagebutton.cpp
    ${GUICHAN_DIR}/src/widgets/label.cpp
    ${GUICHAN_DIR}/src/widgets/listbox.cpp
    ${GUICHAN_DIR}/src/widgets/radiobutton.cpp
    ${GUICHAN_DIR}/src/widgets/scrollarea.cpp
    ${GUICHAN_DIR}/src/widgets/slider.cpp
    ${GUICHAN_DIR}/src/widgets/tab.cpp
    ${GUICHAN_DIR}/src/widgets/tabbedarea.cpp
    ${GUICHAN_DIR}/src/widgets/textbox.cpp
    ${GUICHAN_DIR}/src/widgets/textfield.cpp
    ${GUICHAN_DIR}/src/widgets/window.cpp
)

source_group(extlibs\\guichan FILES ${GUICHAN_SOURCE_FILES})

set( GUICHAN_SFML_DIR "${PROJECT_SOURCE_DIR}/extlibs/guichan-sfml" )
set( GUICHAN_SFML_SOURCE_FILES
    ${GUICHAN_SFML_DIR}/src/guichan/sfml.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlfont.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlgraphics.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlimage.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlimageloader.cpp
    ${GUICHAN_SFML_DIR}/src/guichan/sfml/sfmlinput.cpp
)

source_group(extlibs\\guichan-sfml FILES ${GUICHAN_SFML_SOURCE_FILES})

set( GUICHAN_HAKASE_DIR "${PROJECT_SOURCE_DIR}/extlibs/guichan-hakase" )
set( GUICHAN_HAKASE_SOURCE_FILES
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/fixedimagefont.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functoractionlistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functorselectionlistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functordeathlistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functorfocuslistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/functorwidgetlistener.cpp
    ${GUICHAN_HAKASE_DIR}/src/guichan/hakase/labelex.cpp
)

source_group(extlibs\\guichan-hakase FILES ${GUICHAN_HAKASE_SOURCE_FILES})

set( HIKARI_CLIENT_SOURCE_FILES
    src/hikari/client/Main.cpp
    src/hikari/client/Client.cpp
    src/hikari/client/ClientConfig.cpp
    src/hikari/client/CommandProcessor.cpp
)

source_group(hikari\\client FILES ${HIKARI_CLIENT_SOURCE_FILES})

set( HIKARI_CLIENT_GAME_SOURCE_FILES
    src/hikari/client/game/DamageTable.cpp
    src/hikari/client/game/Effect.cpp
    src/hikari/client/game/GameConfig.cpp
    src/hikari/client/game/GamePlayState.cpp
    src/hikari/client/game/GameOverState.cpp
    src/hikari/client/game/GameProgress.cpp
    src/hikari/client/game/GameWorld.cpp
    src/hikari/client/game/GuiTestState.cpp
    src/hikari/client/game/InputService.cpp
    src/hikari/client/game/KeyboardInput.cpp
    src/hikari/client/game/PasswordState.cpp
    src/hikari/client/game/WeaponGetState.cpp
    src/hikari/client/game/RealTimeInput.cpp
    src/hikari/client/game/Shot.cpp
    src/hikari/client/game/SpawnProjectileWeaponAction.cpp
    src/hikari/client/game/SpriteTestState.cpp
    src/hikari/client/game/StageSelectState.cpp
    src/hikari/client/game/StageSelectStateConfig.cpp
    src/hikari/client/game/ScreenEffectsService.cpp
    src/hikari/client/game/Task.cpp
    src/hikari/client/game/BaseTask.cpp
    src/hikari/client/game/FunctionTask.cpp
    src/hikari/client/game/WaitTask.cpp
    src/hikari/client/game/RefillHealthTask.cpp
    src/hikari/client/game/FadeColorTask.cpp
    src/hikari/client/game/TitleState.cpp
    src/hikari/client/game/OptionsState.cpp
    src/hikari/client/game/Weapon.cpp
    src/hikari/client/game/WeaponAction.cpp
    src/hikari/client/game/WeaponTable.cpp
    src/hikari/client/game/PaletteHelpers.cpp
)

source_group(hikari\\client\\game FILES ${HIKARI_CLIENT_GAME_SOURCE_FILES})

set( HIKARI_CLIENT_GAME_EVENTS_SOURCE_FILES
    src/hikari/client/game/events/BaseEventData.cpp
    src/hikari/client/game/events/DoorEventData.cpp
    src/hikari/client/game/events/EntityDamageEventData.cpp
    src/hikari/client/game/events/EntityDeathEventData.cpp
    src/hikari/client/game/events/EntityStateChangeEventData.cpp
    src/hikari/client/game/events/EventListenerDelegate.cpp
    src/hikari/client/game/events/EventData.cpp
    src/hikari/client/game/events/EventBus.cpp
    src/hikari/client/game/events/EventBusImpl.cpp
    src/hikari/client/game/events/GameQuitEventData.cpp
    src/hikari/client/game/events/ObjectRemovedEventData.cpp
    src/hikari/client/game/events/TransitionCollisionEventData.cpp
    src/hikari/client/game/events/WeaponFireEventData.cpp
    src/hikari/client/game/EventBusService.cpp
)

source_group(hikari\\client\\game\\events FILES ${HIKARI_CLIENT_GAME_EVENTS_SOURCE_FILES})


set( HIKARI_CLIENT_GAME_OBJECTS_SOURCE_FILES
    src/hikari/client/game/objects/GameObject.cpp
    src/hikari/client/game/objects/Hero.cpp
    src/hikari/client/game/objects/AnimatedSprite.cpp
    src/hikari/client/game/objects/PalettedAnimatedSprite.cpp
    src/hikari/client/game/objects/brains/ScriptedEnemyBrain.cpp
    src/hikari/client/game/objects/CollectableItem.cpp
    src/hikari/client/game/objects/controllers/CutSceneHeroActionController.cpp
    src/hikari/client/game/objects/controllers/PlayerInputHeroActionController.cpp
    src/hikari/client/game/objects/effects/NothingEffect.cpp
    src/hikari/client/game/objects/effects/ScriptedEffect.cpp
    src/hikari/client/game/objects/Enemy.cpp
    src/hikari/client/game/objects/EnemyBrain.cpp
    src/hikari/client/game/objects/Projectile.cpp
    src/hikari/client/game/objects/ProjectileFactory.cpp
    src/hikari/client/game/objects/Motion.cpp
    src/hikari/client/game/objects/motions/LinearMotion.cpp
    src/hikari/client/game/objects/Particle.cpp
    src/hikari/client/game/objects/ParticleFactory.cpp
    src/hikari/client/game/objects/Entity.cpp
    src/hikari/client/game/objects/FactoryHelpers.cpp
    src/hikari/client/game/objects/HeroAirbornMobilityState.cpp
    src/hikari/client/game/objects/HeroClimbingMobilityState.cpp
    src/hikari/client/game/objects/HeroDamagedMobilityState.cpp
    src/hikari/client/game/objects/HeroIdleMobilityState.cpp
    src/hikari/client/game/objects/HeroSlidingMobilityState.cpp
    src/hikari/client/game/objects/HeroTeleportingMobilityState.cpp
    src/hikari/client/game/objects/HeroWalkingMobilityState.cpp
    src/hikari/client/game/objects/ItemFactory.cpp
    src/hikari/client/game/objects/ItemSpawner.cpp
    src/hikari/client/game/objects/EnemySpawner.cpp
    src/hikari/client/game/objects/EnemyFactory.cpp
    src/hikari/client/game/objects/Spawner.cpp
)

source_group(hikari\\client\\game\\objects FILES ${HIKARI_CLIENT_GAME_OBJECTS_SOURCE_FILES})

set( HIKARI_CLIENT_SCRIPTING_SOURCE_FILES
    src/hikari/client/scripting/AudioServiceScriptProxy.cpp
    src/hikari/client/scripting/GameProgressScriptProxy.cpp
    src/hikari/client/scripting/GamePlayStateScriptProxy.cpp
    src/hikari/client/scripting/SquirrelService.cpp
    src/hikari/client/scripting/SquirrelUtils.cpp
)

source_group(hikari\\client\\scripting FILES ${HIKARI_CLIENT_SCRIPTING_SOURCE_FILES})

set( HIKARI_CLIENT_AUDIO_SOURCE_FILES
    src/hikari/client/audio/AudioService.cpp
    src/hikari/client/audio/GMESoundStream.cpp
    src/hikari/client/audio/NSFSoundStream.cpp
    src/hikari/client/audio/SoundLibrary.cpp
)

source_group(hikari\\client\\audio FILES ${HIKARI_CLIENT_AUDIO_SOURCE_FILES})

set( HIKARI_CLIENT_GUI_SOURCE_FILES
    src/hikari/client/gui/CommandConsole.cpp
    src/hikari/client/gui/EnergyGauge.cpp
    src/hikari/client/gui/EnergyMeter.cpp
    src/hikari/client/gui/GuiService.cpp
    src/hikari/client/gui/HikariImageLoader.cpp
    src/hikari/client/gui/Panel.cpp
    src/hikari/client/gui/Widget.cpp
    src/hikari/client/gui/MenuItem.cpp
    src/hikari/client/gui/WeaponMenuItem.cpp
    src/hikari/client/gui/Menu.cpp
    src/hikari/client/gui/Icon.cpp
    src/hikari/client/gui/IconAnimator.cpp
    src/hikari/client/gui/InputHelper.cpp
)

source_group(hikari\\client\\gui FILES ${HIKARI_CLIENT_GUI_SOURCE_FILES})

set( HIKARI_CORE_SOURCE_FILES
    src/hikari/core/Hikari.cpp
)

source_group(hikari\\core FILES ${HIKARI_CORE_SOURCE_FILES})

set( HIKARI_CORE_GAME_SOURCE_FILES
    src/hikari/core/game/Animation.cpp
    src/hikari/core/game/AnimationFrame.cpp
    src/hikari/core/game/AnimationLoader.cpp
    src/hikari/core/game/AnimationPlaybackException.cpp
    src/hikari/core/game/AnimationSet.cpp
    src/hikari/core/game/Animator.cpp
    src/hikari/core/game/CollisionInfo.cpp
    src/hikari/core/game/Direction.cpp
    src/hikari/core/game/Faction.cpp
    src/hikari/core/game/StateTransition.cpp
    src/hikari/core/game/FadeStateTransition.cpp
    src/hikari/core/game/SliceStateTransition.cpp
    src/hikari/core/game/GameController.cpp
    src/hikari/core/game/GameControllerException.cpp
    src/hikari/core/game/map/Camera.cpp
    src/hikari/core/game/map/Map.cpp
    src/hikari/core/game/map/MapLoader.cpp
    src/hikari/core/game/map/MapRenderer.cpp
    src/hikari/core/game/map/Door.cpp
    src/hikari/core/game/map/Room.cpp
    src/hikari/core/game/map/RoomTransition.cpp
    src/hikari/core/game/map/Tileset.cpp
    src/hikari/core/game/map/TilesetLoader.cpp
    src/hikari/core/game/Movable.cpp
    src/hikari/core/game/SpriteAnimator.cpp
    src/hikari/core/game/TileAnimator.cpp
    src/hikari/core/game/TileMapCollisionResolver.cpp
    src/hikari/core/game/WorldCollisionResolver.cpp
)

source_group(hikari\\core\\game FILES ${HIKARI_CORE_GAME_SOURCE_FILES})

set( HIKARI_CORE_UTIL_SOURCE_FILES
    src/hikari/core/util/AnimationSetCache.cpp
    src/hikari/core/util/exception/HikariException.cpp
    src/hikari/core/util/exception/ServiceNotRegisteredException.cpp
    src/hikari/core/util/FileSystem.cpp
    src/hikari/core/util/HashedString.cpp
    src/hikari/core/util/Log.cpp
    src/hikari/core/util/Timer.cpp
    src/hikari/core/util/ImageCache.cpp
    src/hikari/core/util/JsonUtil.cpp
    src/hikari/core/util/PhysFS.cpp
    src/hikari/core/util/PhysFSUtils.cpp
    src/hikari/core/util/RedirectStream.cpp
    src/hikari/core/util/StringUtils.cpp
    src/hikari/core/util/TilesetCache.cpp
    src/hikari/core/gui/ImageFont.cpp
)

source_group(hikari\\core\\util FILES ${HIKARI_CORE_UTIL_SOURCE_FILES})

set( HIKARI_CORE_MATH_SOURCE_FILES
    src/hikari/core/math/NESNumber.cpp
)

source_group(hikari\\core\\math FILES ${HIKARI_CORE_MATH_SOURCE_FILES})

set( ENGINE_SOURCE_FILES
    ${HIKARI_CLIENT_SOURCE_FILES}
    ${HIKARI_CLIENT_GAME_SOURCE_FILES}
    ${HIKARI_CLIENT_GAME_EVENTS_SOURCE_FILES}
    ${HIKARI_CLIENT_GAME_OBJECTS_SOURCE_FILES}
    ${HIKARI_CLIENT_SCRIPTING_SOURCE_FILES}
    ${HIKARI_CLIENT_AUDIO_SOURCE_FILES}
    ${HIKARI_CLIENT_GUI_SOURCE_FILES}
    ${HIKARI_CORE_SOURCE_FILES}
    ${HIKARI_CORE_GAME_SOURCE_FILES}
    ${HIKARI_CORE_UTIL_SOURCE_FILES}
    ${HIKARI_CORE_MATH_SOURCE_FILES}
)

set( HIKARI_SOURCE_FILES
    ${GME_SOURCE_FILES}
    ${JSONCPP_SOURCE_FILES}
    ${PHYSFS_STREAM_SOURCE_FILES}
    ${SQUIRREL_FILES}
    ${GUICHAN_SOURCE_FILES}
    ${GUICHAN_SFML_SOURCE_FILES}
    ${GUICHAN_HAKASE_SOURCE_FILES}
    ${ENGINE_SOURCE_FILES}
)

set( INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/engine/include
    ${GME_DIR}
    ${JSONCPP_DIR}/include
    ${JSONCPP_DIR}/src
    ${PHYSFS_STREAM_DIR}
    ${SQUIRREL_DIR}/include
    ${SQUIRREL_DIR}/squirrel
    ${SQUIRREL_DIR}/sqstdlib
    ${SQRAT_DIR}/include
    ${GUICHAN_DIR}/include
    ${GUICHAN_DIR}/include/guichan
    ${GUICHAN_SFML_DIR}/include
    ${GUICHAN_HAKASE_DIR}/include
)

include_directories( ${INCLUDE_DIRS} )

add_executable( hikari ${HIKARI_SOURCE_FILES} ${INCLUDE_DIRS} )

#
# Copy content files to output directory
#
add_custom_target(
  content
  ALL
  COMMAND ${CMAKE_COMMAND} -E echo "Copying content to output directory."
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/content" $<TARGET_FILE_DIR:hikari>
)

#
# We want to link statically on Windows
#
if(WIN32)
    set(SFML_STATIC_LIBRARIES TRUE)
endif(WIN32)

find_package(SFML 2 COMPONENTS graphics window audio system REQUIRED)

if(SFML_FOUND)
    if(APPLE)
        set(OTHER_SFML_LDFLAGS ${OTHER_SFML_LDFLAGS} "-stdlib=libc++")
    endif(APPLE)
    include_directories( ${SFML_INCLUDE_DIR} )
    target_link_libraries( hikari ${SFML_LIBRARIES} ${OTHER_SFML_LDFLAGS} )
endif(SFML_FOUND)

find_package(PhysFS REQUIRED)

if(PHYSFS_FOUND)
    #ADD_DEFINITIONS(-fno-common)
    if(APPLE)
        set(OTHER_LDFLAGS ${OTHER_LDFLAGS} "-framework Carbon -framework IOKit")
    endif(APPLE)
    include_directories( ${PHYSFS_INCLUDE_DIR} )
    target_link_libraries( hikari ${PHYSFS_LIBRARY} ${OTHER_LDFLAGS} )
endif(PHYSFS_FOUND)
