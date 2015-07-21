// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "cc/test/layer_tree_host_common_test.h"

#include "cc/layers/layer.h"
#include "cc/layers/layer_impl.h"
#include "cc/test/fake_layer_tree_host.h"
#include "cc/trees/draw_property_utils.h"
#include "cc/trees/layer_tree_host_common.h"

namespace cc {

LayerTreeHostCommonTestBase::LayerTreeHostCommonTestBase()
    : render_surface_layer_list_count_(0) {
}

LayerTreeHostCommonTestBase::~LayerTreeHostCommonTestBase() {
}

void LayerTreeHostCommonTestBase::SetLayerPropertiesForTesting(
    Layer* layer,
    const gfx::Transform& transform,
    const gfx::Point3F& transform_origin,
    const gfx::PointF& position,
    const gfx::Size& bounds,
    bool flatten_transform,
    bool is_3d_sorted) {
  SetLayerPropertiesForTestingInternal(layer, transform, transform_origin,
                                       position, bounds, flatten_transform,
                                       is_3d_sorted);
}

void LayerTreeHostCommonTestBase::SetLayerPropertiesForTesting(
    LayerImpl* layer,
    const gfx::Transform& transform,
    const gfx::Point3F& transform_origin,
    const gfx::PointF& position,
    const gfx::Size& bounds,
    bool flatten_transform,
    bool is_3d_sorted,
    bool create_render_surface) {
  SetLayerPropertiesForTestingInternal(layer, transform, transform_origin,
                                       position, bounds, flatten_transform,
                                       is_3d_sorted);
  if (create_render_surface) {
    layer->SetHasRenderSurface(true);
  }
}

void LayerTreeHostCommonTestBase::ExecuteCalculateDrawProperties(
    Layer* root_layer,
    float device_scale_factor,
    float page_scale_factor,
    Layer* page_scale_layer,
    bool can_use_lcd_text,
    bool layers_always_allowed_lcd_text) {
  EXPECT_TRUE(page_scale_layer || (page_scale_factor == 1.f));
  gfx::Transform identity_matrix;
  gfx::Size device_viewport_size =
      gfx::Size(root_layer->bounds().width() * device_scale_factor,
                root_layer->bounds().height() * device_scale_factor);

  render_surface_layer_list_.reset(new RenderSurfaceLayerList);

  // We are probably not testing what is intended if the root_layer bounds are
  // empty.
  DCHECK(!root_layer->bounds().IsEmpty());
  LayerTreeHostCommon::CalcDrawPropsMainInputsForTesting inputs(
      root_layer, device_viewport_size, render_surface_layer_list_.get());
  inputs.device_scale_factor = device_scale_factor;
  inputs.page_scale_factor = page_scale_factor;
  inputs.page_scale_layer = page_scale_layer;
  inputs.can_use_lcd_text = can_use_lcd_text;
  inputs.layers_always_allowed_lcd_text = layers_always_allowed_lcd_text;
  inputs.can_adjust_raster_scales = true;
  LayerTreeHostCommon::CalculateDrawProperties(&inputs);
}

void LayerTreeHostCommonTestBase::
    ExecuteCalculateDrawPropertiesWithPropertyTrees(Layer* root_layer) {
  DCHECK(root_layer->layer_tree_host());
  LayerTreeHostCommon::PreCalculateMetaInformation(root_layer);

  gfx::Transform identity_transform;
  bool preserves_2d_axis_alignment = false;
  bool can_render_to_separate_surface = true;
  LayerTreeHostCommon::UpdateRenderSurfaces(
      root_layer, can_render_to_separate_surface, identity_transform,
      preserves_2d_axis_alignment);

  Layer* page_scale_layer = nullptr;
  float page_scale_factor = 1.f;
  float device_scale_factor = 1.f;
  gfx::Size device_viewport_size =
      gfx::Size(root_layer->bounds().width() * device_scale_factor,
                root_layer->bounds().height() * device_scale_factor);
  LayerList update_layer_list;
  BuildPropertyTreesAndComputeVisibleRects(
      root_layer, page_scale_layer, page_scale_factor, device_scale_factor,
      gfx::Rect(device_viewport_size), identity_transform,
      root_layer->layer_tree_host()->property_trees(), &update_layer_list);
}

void LayerTreeHostCommonTestBase::
    ExecuteCalculateDrawPropertiesWithPropertyTrees(LayerImpl* root_layer) {
  DCHECK(root_layer->layer_tree_impl());
  LayerTreeHostCommon::PreCalculateMetaInformationForTesting(root_layer);

  gfx::Transform identity_transform;
  LayerImpl* page_scale_layer = nullptr;
  float page_scale_factor = 1.f;
  float device_scale_factor = 1.f;
  gfx::Size device_viewport_size =
      gfx::Size(root_layer->bounds().width() * device_scale_factor,
                root_layer->bounds().height() * device_scale_factor);
  std::vector<LayerImpl*> update_layer_list;
  BuildPropertyTreesAndComputeVisibleRects(
      root_layer, page_scale_layer, page_scale_factor, device_scale_factor,
      gfx::Rect(device_viewport_size), identity_transform,
      root_layer->layer_tree_impl()->property_trees(), &update_layer_list);
}

void LayerTreeHostCommonTestBase::ExecuteCalculateDrawProperties(
    LayerImpl* root_layer,
    float device_scale_factor,
    float page_scale_factor,
    LayerImpl* page_scale_layer,
    bool can_use_lcd_text,
    bool layers_always_allowed_lcd_text) {
  gfx::Transform identity_matrix;
  gfx::Size device_viewport_size =
      gfx::Size(root_layer->bounds().width() * device_scale_factor,
                root_layer->bounds().height() * device_scale_factor);

  render_surface_layer_list_impl_.reset(new LayerImplList);

  // We are probably not testing what is intended if the root_layer bounds are
  // empty.
  DCHECK(!root_layer->bounds().IsEmpty());
  LayerTreeHostCommon::CalcDrawPropsImplInputsForTesting inputs(
      root_layer, device_viewport_size, render_surface_layer_list_impl_.get());
  inputs.device_scale_factor = device_scale_factor;
  inputs.page_scale_factor = page_scale_factor;
  inputs.page_scale_layer = page_scale_layer;
  inputs.can_use_lcd_text = can_use_lcd_text;
  inputs.layers_always_allowed_lcd_text = layers_always_allowed_lcd_text;
  inputs.can_adjust_raster_scales = true;

  ++render_surface_layer_list_count_;
  inputs.current_render_surface_layer_list_id =
      render_surface_layer_list_count_;

  LayerTreeHostCommon::CalculateDrawProperties(&inputs);
}

}  // namespace cc