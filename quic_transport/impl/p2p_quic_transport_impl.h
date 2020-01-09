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

#ifndef OWT_QUIC_TRANSPORT_P2P_QUIC_TRANSPORT_IMPL_H_
#define OWT_QUIC_TRANSPORT_P2P_QUIC_TRANSPORT_IMPL_H_

#include "base/at_exit.h"
#include "net/quic/quic_chromium_connection_helper.h"
#include "net/third_party/quiche/src/quic/core/crypto/quic_compressed_certs_cache.h"
#include "net/third_party/quiche/src/quic/core/crypto/quic_crypto_client_config.h"
#include "net/third_party/quiche/src/quic/quartc/quartc_factory.h"
#include "net/third_party/quiche/src/quic/quartc/quartc_session.h"
#include "owt/quic/p2p_quic_packet_transport_interface.h"
#include "owt/quic/p2p_quic_transport_interface.h"
#include "owt/quic/quic_definitions.h"
#include "third_party/webrtc/api/scoped_refptr.h"
#include "third_party/webrtc/rtc_base/rtc_certificate.h"

namespace owt {
namespace quic {
// Some ideas of this class are borrowed from
// src/third_party/blink/renderer/modules/peerconnection/adapters/p2p_quic_transport_impl.h.
// It always acts as a server side endpoint.
class P2PQuicTransportImpl
    : public P2PQuicTransportInterface,
      public P2PQuicPacketTransportInterface::ReceiveDelegate,
      public ::quic::QuicSession::Visitor {
 public:
  static std::unique_ptr<P2PQuicTransportImpl> Create(
      const ::quic::QuartcSessionConfig& quartcSessionConfig,
      ::quic::Perspective perspective,
      std::shared_ptr<::quic::QuartcPacketTransport> transport,
      ::quic::QuicClock* clock,
      std::shared_ptr<::quic::QuicAlarmFactory> alarmFactory,
      std::shared_ptr<::quic::QuicConnectionHelperInterface> helper,
      std::shared_ptr<::quic::QuicCryptoServerConfig> cryptoServerConfig,
      ::quic::QuicCompressedCertsCache* const compressedCertsCache,
      base::TaskRunner* runner);
  virtual std::vector<rtc::scoped_refptr<rtc::RTCCertificate>> GetCertificates()
      const;
  virtual void Start(std::unique_ptr<RTCQuicParameters> remoteParameters);
  // virtual void listen(const std::string& remoteKey);
  RTCQuicParameters GetLocalParameters() const override;

  void SetDelegate(P2PQuicTransportInterface::Delegate* delegate) {
    m_delegate = delegate;
  }

  explicit P2PQuicTransportImpl(
      owt::quic::P2PQuicPacketTransportInterface* quic_packet_transport,
      const ::quic::QuicConfig& quic_config,
      const ::quic::QuicCryptoServerConfig* crypto_config,
      ::quic::QuicCompressedCertsCache* const compressed_certs_cache,
      ::quic::QuicClock* clock,
      ::quic::QuicAlarmFactory* alarm_factory,
      ::quic::QuicConnectionHelperInterface* connection_helper,
      base::TaskRunner* runner);
  ~P2PQuicTransportImpl() override;

 protected:
  void OnPacketDataReceived(const char* data, size_t data_len) override;

  // Implementes quic::QuicSession::Visitor.
  // Called when the connection is closed after the streams have been closed.
  void OnConnectionClosed(::quic::QuicConnectionId server_connection_id,
                          ::quic::QuicErrorCode error,
                          const std::string& error_details,
                          ::quic::ConnectionCloseSource source) override {}
  // Called when the session has become write blocked.
  void OnWriteBlocked(
      ::quic::QuicBlockedWriterInterface* blocked_writer) override {}
  // Called when the session receives reset on a stream from the peer.
  void OnRstStreamReceived(const ::quic::QuicRstStreamFrame& frame) override {}
  // Called when the session receives a STOP_SENDING for a stream from the
  // peer.
  void OnStopSendingReceived(
      const ::quic::QuicStopSendingFrame& frame) override {}

 private:
  static std::unique_ptr<::quic::QuicConnection> CreateQuicConnection(
      ::quic::Perspective perspective,
      std::shared_ptr<::quic::QuartcPacketWriter> writer,
      std::shared_ptr<::quic::QuicAlarmFactory> alarmFactory,
      std::shared_ptr<::quic::QuicConnectionHelperInterface> connectionHelper);
  std::shared_ptr<::quic::QuartcPacketWriter> m_writer;
  std::shared_ptr<::quic::QuicCryptoServerConfig> m_cryptoServerConfig;
  P2PQuicTransportInterface::Delegate* m_delegate;
  std::vector<std::unique_ptr<P2PQuicStreamInterface>> m_streams;
  base::TaskRunner* m_runner;
  std::unique_ptr<::quic::QuartcPacketTransport> quartc_packet_transport_;
  std::unique_ptr<::quic::QuartcPacketWriter> quartc_packet_writer_;
  std::unique_ptr<::quic::QuartcServerSession> quartc_session_;
  ::quic::QuicClock* clock_;
};
}  // namespace quic
}  // namespace owt

#endif