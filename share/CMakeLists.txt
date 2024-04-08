if(SVSCRAFT_BUILD_TRANSLATIONS)
    qm_import(Translate)
    qm_find_qt(LinguistTools)

    add_custom_target(svscraft_lupdate_task)
    add_custom_target(svscraft_lrelease_task)

    # svscraft
    get_property(_targets GLOBAL PROPERTY SVSCRAFT_TRANSLATE_TARGETS)
    set(_qm_dir ${CMAKE_BUILD_SHARE_DIR}/${SVSCRAFT_INSTALL_NAME}/translations)
    qm_add_translation(svscraft_translations
        LOCALES ja_JP zh_CN zh_TW
        PREFIX svscraft
        TARGETS ${_targets}
        TS_DIR translations
        QM_DIR ${_qm_dir}
        TS_DEPENDS svscraft_lupdate_task
        QM_DEPENDS svscraft_lrelease_task
    )

    if(SVSCRAFT_INSTALL)
        install(CODE "
            # execute_process(COMMAND \"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --target svscraft_lupdate_task)
            execute_process(COMMAND \"${CMAKE_COMMAND}\" --build \"${CMAKE_BINARY_DIR}\" --target svscraft_lrelease_task)
        ")
        install(DIRECTORY ${_qm_dir}/
            DESTINATION ${CMAKE_INSTALL_DATADIR}/${SVSCRAFT_INSTALL_NAME}/translations
            FILES_MATCHING PATTERN "*.qm"
        )
    endif()
endif()