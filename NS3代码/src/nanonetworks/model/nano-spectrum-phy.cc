/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013,2014 TELEMATICS LAB, DEI - Politecnico di Bari
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Giuseppe Piro <peppe@giuseppepiro.com>, <g.piro@poliba.it>
 */

#include "ns3/waveform-generator.h"
#include "ns3/object-factory.h"
#include "ns3/log.h"
#include "math.h"
#include "ns3/simulator.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/packet-burst.h"
#include "ns3/callback.h"
#include "ns3/spectrum-error-model.h"
#include "nano-spectrum-phy.h"
#include "nano-spectrum-value-helper.h"
#include <ns3/antenna-model.h>

NS_LOG_COMPONENT_DEFINE("NanoSpectrumPhy");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(NanoSpectrumPhy);

NanoSpectrumPhy::NanoSpectrumPhy() {
}

NanoSpectrumPhy::~NanoSpectrumPhy() {
}

void NanoSpectrumPhy::Initialize(void) {
	NS_LOG_FUNCTION(this);
	m_mobility = 0;
	m_netDevice = 0;
	m_channel = 0;
	m_txPsd = 0;
	m_rxPsd = 0;
	m_interference.SetErrorModel(0);
}

void NanoSpectrumPhy::DoDispose() {
	NS_LOG_FUNCTION(this);
	m_mobility = 0;
	m_netDevice = 0;
	m_channel = 0;
	m_txPsd = 0;
	m_rxPsd = 0;
	m_phyMacTxEndCallback = MakeNullCallback<void, Ptr<const Packet> >();
	m_phyMacRxStartCallback = MakeNullCallback<void>();
	m_phyMacRxEndErrorCallback = MakeNullCallback<void>();
	m_phyMacRxEndOkCallback = MakeNullCallback<void, Ptr<Packet> >();
	SpectrumPhy::DoDispose();
}

TypeId NanoSpectrumPhy::GetTypeId(void) {
	static TypeId tid = TypeId("ns3::NanoSpectrumPhy").SetParent<SpectrumPhy>();
	return tid;
}

Ptr<NetDevice> NanoSpectrumPhy::GetDevice() const {
	NS_LOG_FUNCTION(this);
	return m_netDevice;
}

Ptr<MobilityModel> NanoSpectrumPhy::GetMobility() {
	NS_LOG_FUNCTION(this);
	return m_mobility;
}

void NanoSpectrumPhy::SetDevice(Ptr<NetDevice> d) {
	NS_LOG_FUNCTION(this << d);
	m_netDevice = d;
}

void NanoSpectrumPhy::SetMobility(Ptr<MobilityModel> m) {
	NS_LOG_FUNCTION(this << m);
	m_mobility = m;
}

void NanoSpectrumPhy::SetChannel(Ptr<SpectrumChannel> c) {
	NS_LOG_FUNCTION(this << c);
	m_channel = c;
}

Ptr<const SpectrumModel> NanoSpectrumPhy::GetRxSpectrumModel() const {
	if (m_txPsd) {
		return m_txPsd->GetSpectrumModel();
	} else {
		return 0;
	}
}

void NanoSpectrumPhy::SetTxPower(double p) {
	NS_LOG_FUNCTION(this << p);
	m_txPower = p;
	NanoSpectrumValueHelper helper;
	Ptr<SpectrumValue> txPsd = helper.CreateTxPowerSpectralDensity(p);
	SetTxPowerSpectralDensity(txPsd);
}

void NanoSpectrumPhy::SetTxPowerSpectralDensity(Ptr<SpectrumValue> txPsd) {
	NS_LOG_FUNCTION(this << txPsd);
	NS_ASSERT(txPsd);
	m_txPsd = txPsd;
	NS_LOG_INFO(*txPsd << *m_txPsd);
}

Ptr<SpectrumValue> NanoSpectrumPhy::GetTxPowerSpectralDensity(void) {
	NS_LOG_FUNCTION(this);
	return m_txPsd;
}

Ptr<SpectrumChannel> NanoSpectrumPhy::GetChannel(void) {
	NS_LOG_FUNCTION(this);
	return m_channel;
}

void NanoSpectrumPhy::SetNoisePowerSpectralDensity(Ptr<const SpectrumValue> noisePsd) {
	NS_LOG_FUNCTION(this << noisePsd);
	NS_ASSERT(noisePsd);
	m_interference.SetNoisePowerSpectralDensity(noisePsd);
}

void NanoSpectrumPhy::SetGenericPhyTxEndCallback(GenericPhyTxEndCallback c) {
	NS_LOG_FUNCTION(this);
	m_phyMacTxEndCallback = c;
}

void NanoSpectrumPhy::SetGenericPhyRxStartCallback(GenericPhyRxStartCallback c) {
	NS_LOG_FUNCTION(this);
	m_phyMacRxStartCallback = c;
}

void NanoSpectrumPhy::SetGenericPhyRxEndErrorCallback(GenericPhyRxEndErrorCallback c) {
	NS_LOG_FUNCTION(this);
	m_phyMacRxEndErrorCallback = c;
}

void NanoSpectrumPhy::SetGenericPhyRxEndOkCallback(GenericPhyRxEndOkCallback c) {
	NS_LOG_FUNCTION(this);
	m_phyMacRxEndOkCallback = c;
}

Ptr<AntennaModel> NanoSpectrumPhy::GetRxAntenna() {
	return NULL;
}

void NanoSpectrumPhy::SetState(State newState) {
	NS_LOG_FUNCTION(this << "from " << m_state << " to " << newState);
	m_state = newState;
}

NanoSpectrumPhy::State NanoSpectrumPhy::GetState() {
	return m_state;
}

void NanoSpectrumPhy::SetTransmissionRange(double r) {
	NS_LOG_FUNCTION(this << r);
	m_transmissionRange = r;
}

double NanoSpectrumPhy::GetTransmissionRange() {
	NS_LOG_FUNCTION(this);
	return m_transmissionRange;
}

} // namespace ns3
