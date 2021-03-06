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


#include "message-process-unit.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "simple-nano-device.h"
#include "nano-mac-queue.h"
#include "nano-spectrum-phy.h"
#include "nano-mac-header.h"
#include "nano-seq-ts-header.h"
#include "ns3/simulator.h"
#include "ns3/nano-l3-header.h"

NS_LOG_COMPONENT_DEFINE ("MessageProcessUnit");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (MessageProcessUnit);

TypeId MessageProcessUnit::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MessageProcessUnit")
    .SetParent<Object> ()
    .AddTraceSource ("outTX",  "outTX",  MakeTraceSourceAccessor (&MessageProcessUnit::m_outTX),
                     "ns3::MessageProcessUnit::OutTxCallback")
    .AddTraceSource ("outRX",  "outRX",  MakeTraceSourceAccessor (&MessageProcessUnit::m_outRX),
                     "ns3::MessageProcessUnit::OutRxCallback");
;
  return tid;
}


MessageProcessUnit::MessageProcessUnit ()
{
  NS_LOG_FUNCTION (this);
  m_device = 0;
  m_packetSize = 0;
  m_interarrivalTime = 99999999999;
}


MessageProcessUnit::~MessageProcessUnit ()
{
  NS_LOG_FUNCTION (this);
}

void  MessageProcessUnit::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  m_device = 0;
}

void MessageProcessUnit::SetDevice (Ptr<SimpleNanoDevice> d)
{
  NS_LOG_FUNCTION (this);
  m_device = d;
}

Ptr<SimpleNanoDevice> MessageProcessUnit::GetDevice (void)
{
  return m_device;
}

void MessageProcessUnit::CreteMessage ()				//??????????????????
{
	NS_LOG_FUNCTION (this);
	uint8_t *buffer  = new uint8_t[m_packetSize];				//????????????102??????100
	for (int i = 0; i < m_packetSize; i++)
	{
		buffer[i] = 129;
	}
	Ptr<Packet> p = Create<Packet>(buffer, m_packetSize);
	NanoSeqTsHeader seqTs;				//seqTs???size???12??????
	seqTs.SetSeq (p->GetUid ());
	p->AddHeader (seqTs);				//p->GetSize()????????????????????????+12
	m_outTX ((int)GetDevice ()->GetNode ()->GetId (), (int)p->GetUid ());
	//std::cout << "creteP:" << p->GetUid() << " " << p->GetSize() << " " << "m_packetSize:" << m_packetSize << std::endl;
	m_device->SendPacket (p);
	Simulator::Schedule (Seconds (m_interarrivalTime), &MessageProcessUnit::CreteMessage, this);		//m_interarrivalTime=0.1
}

void MessageProcessUnit::computeaverindex ()					//???????????????????????????0.01s?????????????????????????????????
{
	GetDevice ()->GetMac ()->CheckForNeighborss();
	std::vector<NanoDetail> neighbors = GetDevice ()->GetMac ()->m_neighborss;
	uint32_t sum = 0;
	double average = 0;
	double varaverage = 0;
	if (neighbors.size() != 0)
	{
		GetDevice()->ConsumeEnergySend(GetDevice()->GetTestSize());
		std::vector<NanoDetail>::iterator it;
		for (it = neighbors.begin (); it != neighbors.end (); it++)
		{
			GetDevice()->ConsumeEnergyReceive(GetDevice()->GetTestSize());
			sum = sum + (*it).in_index;
		}
		average = sum * 1.0 / neighbors.size();					//????????????????????????index???
	}
	varaverage = (average - GetDevice()->GetaverageIndex()) * 1.0 / 0.01;			//????????????index???????????????????????????????????????????????????????????????
	GetDevice()->SetaverageIndex(average);
	GetDevice()->Setmobility(varaverage);						//???????????????????????????????????????
	//std::cout<<GetDevice()->Getmobile()<<"  "<<GetDevice()->GetaverageIndex()<<std::endl;
    Simulator::Schedule (Seconds (0.01), &MessageProcessUnit::computeaverindex, this);			//??????0.1??????????????????????????????????????????,m_interarrivalTime=0.1??????????????????,
}

void MessageProcessUnit::CretetestMessage ()			//??????????????????????????????????????????????????????
{
    NS_LOG_FUNCTION (this);
	uint8_t *buffer  = new uint8_t[m_testPacketSize];				//????????????100??????50
	for (int i = 0; i < m_testPacketSize; i++)
	{
	  buffer[i] = 0;
	}
	Ptr<Packet> p = Create<Packet>(buffer, m_testPacketSize);
	NanoSeqTsHeader seqTs;					//seqTs???size???12
	seqTs.SetSeq (p->GetUid ());
	p->AddHeader (seqTs);
	//std::cout << "testP:" << p->GetUid() << " " << p->GetSize() << " " << "m_testPacketSize:" << m_testPacketSize << std::endl;
	m_device->SendtestPacket (p);			//p->GetSize()????????????????????????+12
    Simulator::Schedule (Seconds (m_interarrivalTestTime), &MessageProcessUnit::CretetestMessage, this);		//??????0.01??????????????????????????????????????????
}

void MessageProcessUnit::ProcessMessage (Ptr<Packet> p)
{
	NS_LOG_FUNCTION (this);

	NanoL3Header l3Header;
	p->RemoveHeader (l3Header);

	NanoSeqTsHeader seqTs;
	p->RemoveHeader (seqTs);				//?????????seqTs???????????????????????????seqTs??????
	NS_LOG_FUNCTION (this << l3Header);
	NS_LOG_FUNCTION (this << seqTs);

	double delay = Simulator::Now ().GetPicoSeconds () - seqTs.GetTs ().GetPicoSeconds ();			//seqTs????????????????????????????????????????????????????????????????????????
	//std::cout<<Simulator::Now ().GetPicoSeconds ()-seqTs.GetTs ().GetPicoSeconds ()<<std::endl;
	//std::cout<<l3Header.GetSource()<<"  "<<l3Header.GetDestination()<<std::endl;
	//std::cout<<seqTs.GetSeq ()<<"  "<< p->GetSize ()<<"  "<<(int)l3Header.GetSource ()<<"  "<<(int)GetDevice ()->GetNode ()->GetId ()<<"  "<<delay<<std::endl;
	m_outRX (seqTs.GetSeq (), (int)l3Header.GetSource (), (int)GetDevice ()->GetNode ()->GetId (), p->GetSize (), delay);
}


void MessageProcessUnit::SetPacketSize (int s)
{
	NS_LOG_FUNCTION (this);
	m_packetSize = s;
}

void MessageProcessUnit::SetTestPacketSize (int s)
{
	NS_LOG_FUNCTION (this);
	m_testPacketSize = s;
}

void MessageProcessUnit::SetInterarrivalTime (double t)
{
	NS_LOG_FUNCTION (this);
	m_interarrivalTime = t;
}

void MessageProcessUnit::SetInterarrivalTestTime (double t)
{
	NS_LOG_FUNCTION (this);
	m_interarrivalTestTime = t;
}

} // namespace ns3
