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


#include "nano-helper.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include <string>
#include "ns3/config.h"
#include "ns3/single-model-spectrum-channel.h"
#include "ns3/spectrum-propagation-loss-model.h"
#include "ns3/ts-ook-based-nano-spectrum-phy.h"
#include "ns3/nano-routing-entity.h"
#include "ns3/flooding-nano-routing-entity.h"
#include "ns3/nanointerface-nano-device.h"
#include "ns3/nanorouter-nano-device.h"
#include "ns3/nanonode-nano-device.h"
#include "ns3/random-nano-routing-entity.h"
#include "ns3/flooding-nano-routing-entity.h"

NS_LOG_COMPONENT_DEFINE ("NanoHelper");

namespace ns3 {

NanoHelper::NanoHelper (void)
  : m_channel (CreateObject<NanoSpectrumChannel> ())
{}

NanoHelper::~NanoHelper (void)
{
  m_channel = 0;
}


void 
NanoHelper::AddMobility (Ptr<NanoSpectrumPhy> phy, Ptr<MobilityModel> m)
{
  phy->SetMobility (m);
}


NetDeviceContainer 
NanoHelper::Install (NodeContainer c)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
    {
      Ptr<Node> node = *i;
      Ptr<NanoSpectrumPhy> phy;
      Ptr<TsOokBasedNanoSpectrumPhy> phy2 = CreateObject<TsOokBasedNanoSpectrumPhy>();
      phy = phy2;
      Ptr<SimpleNanoDevice> device = CreateObject<SimpleNanoDevice>();
      device->SetNode (node);
      device->SetPhy (phy);
      //device->SetAddress (Mac48Address::Allocate ());
      phy->SetDevice (device);
      //device->Start ();
      node->AddDevice (device);
      devices.Add (device);

      /*
      Ptr<FloodingNanoRoutingEntity> l3 = CreateObject<FloodingNanoRoutingEntity>();
      l3->SetDevice (device);
      device->SetL3 (l3);
      */

      phy->SetChannel (m_channel);
      m_channel->AddRx (phy);
    }
  return devices;
}


NetDeviceContainer
NanoHelper::Install (NodeContainer c, NanoHelper::NodeType t)
{
  NetDeviceContainer devices;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); i++)
	{
	  Ptr<Node> node = *i;
	  Ptr<NanoSpectrumPhy> phy;
	  Ptr<TsOokBasedNanoSpectrumPhy> phy2 = CreateObject<TsOokBasedNanoSpectrumPhy>();
	  phy = phy2;
	  Ptr<SimpleNanoDevice> device;

	  if (t == NanoHelper::nanonode)
	    {
		  Ptr<NanoNodeDevice> device2 = CreateObject<NanoNodeDevice> ();
		  device = device2;
	    }
	  else if (t == NanoHelper::nanorouter)
	    {
		  Ptr<NanoRouterDevice> device2 = CreateObject<NanoRouterDevice> ();
		  device = device2;
	    }
	  else if (t == NanoHelper::nanointerface)
	    {
		  Ptr<NanoInterfaceDevice> device2 = CreateObject<NanoInterfaceDevice> ();
		  device = device2;
	    }

	  device->SetNode (node);
	  device->SetPhy (phy);
	  //device->SetAddress (Mac48Address::Allocate ());
	  phy->SetDevice (device);
	  //device->Start ();
	  node->AddDevice (device);
	  devices.Add (device);

	  Ptr<NanoRoutingEntity> l3 = CreateObject<NanoRoutingEntity>();
	  l3->SetDevice (device);
	  device->SetL3 (l3);

	  phy->SetChannel (m_channel);
	  m_channel->AddRx (phy);
	}
  return devices;

}

void 
NanoHelper::EnableLogComponents (void)
{
  LogComponentEnable ("NanoMacEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("BackoffBasedNanoMacEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("TransparentNanoMacEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("NanoMacQueue", LOG_LEVEL_ALL);
  LogComponentEnable ("NanoRoutingEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("FloodingNanoRoutingEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("RandomNanoRoutingEntity", LOG_LEVEL_ALL);
  LogComponentEnable ("NanoSpectrumChannel", LOG_LEVEL_ALL);
  LogComponentEnable ("NanoSpectrumPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("TsOokBasedNanoSpectrumPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("SimpleNanoDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("NanoNodeDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("NanoRouterDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("NanoInterfaceDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("MessageProcessUnit", LOG_LEVEL_ALL);
}


} // namespace ns3
