SET(SOURCES screens/ui_Index.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    fonts/ui_font_Kanit22.c
    fonts/ui_font_Kanit18.c
    fonts/ui_font_Kanit18.c)

add_library(ui ${SOURCES})
