SET(SOURCES 
    ui_comp_progress.c
    ui_comp.c
    ui_Screen1.c
    ui_Screen_Settings.c
    ui.c
    ui_comp_hook.c
    ui_helpers.c
    ui_font_Arial_100.c
    ui_font_Arial_110.c
    ui_events.c)

# Exclude directories from build
set_property(GLOBAL PROPERTY CPACK_IGNORE_FILES 
    "attached_assets/*"
    "factory_data/*"
)

add_library(ui ${SOURCES})
