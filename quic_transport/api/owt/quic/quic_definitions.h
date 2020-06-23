/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Most classes in this file and its implementations are borrowed from
// Chromium/src/third_party/blink/renderer/modules/peerconnection/adapters/*
// with modifications.

#ifndef OWT_QUIC_TRANSPORT_QUIC_DEFINITIONS_H_
#define OWT_QUIC_TRANSPORT_QUIC_DEFINITIONS_H_

#include "owt/quic/export.h"

namespace owt {
namespace quic {

enum class OWT_EXPORT QuicTransportState : uint8_t {
  kNew,
  kConnecting,
  kConnected,
  kClosed,
  kFailed
};

}  // namespace quic
}  // namespace owt

#endif