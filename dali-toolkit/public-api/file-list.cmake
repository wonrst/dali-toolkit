# This file is auto-generated!
#

# Set the source directory

SET( public_api_src_dir ${ROOT_SRC_DIR}/dali-toolkit/public-api )

# Add local source files here
SET( public_api_src_files
  ${public_api_src_dir}/controls/control-impl.cpp
  ${public_api_src_dir}/controls/control.cpp
  ${public_api_src_dir}/controls/alignment/alignment.cpp
  ${public_api_src_dir}/controls/buttons/button.cpp
  ${public_api_src_dir}/controls/buttons/check-box-button.cpp
  ${public_api_src_dir}/controls/buttons/push-button.cpp
  ${public_api_src_dir}/controls/buttons/radio-button.cpp
  ${public_api_src_dir}/controls/flex-container/flex-container.cpp
  ${public_api_src_dir}/controls/image-view/image-view.cpp
  ${public_api_src_dir}/controls/model3d-view/model3d-view.cpp
  ${public_api_src_dir}/controls/progress-bar/progress-bar.cpp
  ${public_api_src_dir}/controls/scroll-bar/scroll-bar.cpp
  ${public_api_src_dir}/controls/scrollable/item-view/default-item-layout.cpp
  ${public_api_src_dir}/controls/scrollable/item-view/item-layout.cpp
  ${public_api_src_dir}/controls/scrollable/item-view/item-view.cpp
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view-constraints.cpp
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view-effect.cpp
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view-page-path-effect.cpp
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view.cpp
  ${public_api_src_dir}/controls/scrollable/scrollable.cpp
  ${public_api_src_dir}/controls/slider/slider.cpp
  ${public_api_src_dir}/controls/table-view/table-view.cpp
  ${public_api_src_dir}/controls/text-controls/text-editor.cpp
  ${public_api_src_dir}/controls/text-controls/text-label.cpp
  ${public_api_src_dir}/controls/text-controls/text-field.cpp
  ${public_api_src_dir}/controls/video-view/video-view.cpp
  ${public_api_src_dir}/image-loader/async-image-loader.cpp
  ${public_api_src_dir}/image-loader/sync-image-loader.cpp
  ${public_api_src_dir}/styling/style-manager.cpp
  ${public_api_src_dir}/accessibility-manager/accessibility-manager.cpp
  ${public_api_src_dir}/focus-manager/keyboard-focus-manager.cpp
  ${public_api_src_dir}/dali-toolkit-version.cpp
  ${public_api_src_dir}/enums.cpp
)

# Add public header files here
SET( public_api_header_files
  ${public_api_src_dir}/dali-toolkit-version.h
  ${public_api_src_dir}/dali-toolkit-common.h
  ${public_api_src_dir}/enums.h
  ${public_api_src_dir}/align-enumerations.h
  ${public_api_src_dir}/toolkit-property-index-ranges.h
)


SET( public_api_controls_header_files
  ${public_api_src_dir}/controls/control-impl.h
  ${public_api_src_dir}/controls/control.h
)


SET( public_api_alignment_header_files
  ${public_api_src_dir}/controls/alignment/alignment.h
)


SET( public_api_buttons_header_files
  ${public_api_src_dir}/controls/buttons/button.h
  ${public_api_src_dir}/controls/buttons/check-box-button.h
  ${public_api_src_dir}/controls/buttons/push-button.h
  ${public_api_src_dir}/controls/buttons/radio-button.h
)


SET( public_api_model3d_view_header_files
  ${public_api_src_dir}/controls/model3d-view/model3d-view.h
)


SET( public_api_flex_container_header_files
  ${public_api_src_dir}/controls/flex-container/flex-container.h
)


SET( public_api_image_view_header_files
  ${public_api_src_dir}/controls/image-view/image-view.h
)


SET( public_api_item_view_header_files
  ${public_api_src_dir}/controls/scrollable/item-view/default-item-layout.h
  ${public_api_src_dir}/controls/scrollable/item-view/default-item-layout-property.h
  ${public_api_src_dir}/controls/scrollable/item-view/item-factory.h
  ${public_api_src_dir}/controls/scrollable/item-view/item-layout.h
  ${public_api_src_dir}/controls/scrollable/item-view/item-view-declarations.h
  ${public_api_src_dir}/controls/scrollable/item-view/item-view.h
)


SET( public_api_image_loader_header_files
  ${public_api_src_dir}/image-loader/async-image-loader.h
  ${public_api_src_dir}/image-loader/sync-image-loader.h
)


SET( public_api_progress_bar_header_files
  ${public_api_src_dir}/controls/progress-bar/progress-bar.h
)


SET( public_api_scrollable_header_files
  ${public_api_src_dir}/controls/scrollable/scrollable.h
)


SET( public_api_scroll_bar_header_files
  ${public_api_src_dir}/controls/scroll-bar/scroll-bar.h
)


SET( public_api_scroll_view_header_files
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-mode.h
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view-constraints.h
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view-effect.h
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view-page-path-effect.h
  ${public_api_src_dir}/controls/scrollable/scroll-view/scroll-view.h
)


SET( public_api_slider_header_files
  ${public_api_src_dir}/controls/slider/slider.h
)


SET( public_api_styling_header_files
  ${public_api_src_dir}/styling/style-manager.h
)


SET( public_api_table_view_header_files
  ${public_api_src_dir}/controls/table-view/table-view.h
)


SET( public_api_text_controls_header_files
  ${public_api_src_dir}/controls/text-controls/hidden-input-properties.h
  ${public_api_src_dir}/controls/text-controls/placeholder-properties.h
  ${public_api_src_dir}/controls/text-controls/text-editor.h
  ${public_api_src_dir}/controls/text-controls/text-label.h
  ${public_api_src_dir}/controls/text-controls/text-field.h
)


SET( public_api_accessibility_manager_header_files
  ${public_api_src_dir}/accessibility-manager/accessibility-manager.h
)


SET( public_api_focus_manager_header_files
  ${public_api_src_dir}/focus-manager/keyboard-focus-manager.h
)


SET( public_api_text_header_files
  ${public_api_src_dir}/text/rendering-backend.h
  ${public_api_src_dir}/text/text-enumerations.h
)


SET( public_api_video_view_header_files
  ${public_api_src_dir}/controls/video-view/video-view.h
)


SET( public_api_visuals_header_files
  ${public_api_src_dir}/visuals/border-visual-properties.h
  ${public_api_src_dir}/visuals/color-visual-properties.h
  ${public_api_src_dir}/visuals/gradient-visual-properties.h
  ${public_api_src_dir}/visuals/image-visual-properties.h
  ${public_api_src_dir}/visuals/mesh-visual-properties.h
  ${public_api_src_dir}/visuals/primitive-visual-properties.h
  ${public_api_src_dir}/visuals/visual-properties.h
  ${public_api_src_dir}/visuals/text-visual-properties.h
)


SET( SOURCES ${SOURCES}
  ${public_api_src_files}
)

SET( PUBLIC_API_HEADERS ${PUBLIC_API_HEADERS}
  ${public_api_header_files}
  ${public_api_controls_header_files}
  ${public_api_alignment_header_files}
  ${public_api_buttons_header_files}
  ${public_api_model3d_view_header_files}
  ${public_api_flex_container_header_files}
  ${public_api_image_view_header_files}
  ${public_api_item_view_header_files}
  ${public_api_image_loader_header_files}
  ${public_api_progress_bar_header_files}
  ${public_api_scrollable_header_files}
  ${public_api_scroll_bar_header_files}
  ${public_api_scroll_view_header_files}
  ${public_api_slider_header_files}
  ${public_api_styling_header_files}
  ${public_api_table_view_header_files}
  ${public_api_text_controls_header_files}
  ${public_api_accessibility_manager_header_files}
  ${public_api_focus_manager_header_files}
  ${public_api_text_header_files}
  ${public_api_video_view_header_files}
  ${public_api_visuals_header_files}
)
