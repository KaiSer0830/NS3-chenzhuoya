/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013,2014 TELEMATICS LAB, DEE - Politecnico di Bari
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



#include "fsa-nano-mac-entity.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"
#include "simple-nano-device.h"
#include "nano-mac-queue.h"
#include "nano-spectrum-phy.h"
#include "nano-mac-header.h"
#include "ns3/seq-ts-header.h"
#include "ns3/simulator.h"
#include "nano-routing-entity.h"


NS_LOG_COMPONENT_DEFINE ("FSANanoMacEntity");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (FSANanoMacEntity);

TypeId FSANanoMacEntity::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::FSANanoMacEntity").SetParent<NanoMacEntity> ();
	return tid;
}


FSANanoMacEntity::FSANanoMacEntity ()
{
	SetMacQueue (CreateObject <NanoMacQueue> ());
	SetMacQueue1(CreateObject <NanoMacQueue> ());
	SetMacQueue2(CreateObject <NanoMacQueue> ());
	SetDevice (0);
	Simulator::Schedule (Seconds(0.001), &NanoMacEntity::CheckForNeighbors, this);
}


FSANanoMacEntity::~FSANanoMacEntity ()
{
}

void FSANanoMacEntity::DoDispose (void)
{
	NanoMacEntity::DoDispose ();
}


void FSANanoMacEntity::DoSendPacket ()			//处理节点数据包与网关探测数据包
{
	NS_LOG_FUNCTION (this);
	Ptr<NanoSpectrumPhy> phy = GetDevice ()->GetPhy ();

	//srand ( time(NULL) );
	Ptr<Packet> p = (m_queue.front ())->Copy ();		//从m_queue队列中复制第一个数据包
	m_queue.pop_front ();								//m_queue队列中删除第一个数据包
	phy->StartTx (p);
	if (m_queue.size () > 0)							//m_queue.size ()不会大于0，进不去该判断，用不上退避时隙等待
	{
		double backoff = rand () % 100000;				//rand ()小于100000，所有退避时隙在0-100ns
		Simulator::Schedule (PicoSeconds (backoff), &FSANanoMacEntity::DoSendPacket, this);
	}
}

void FSANanoMacEntity::sendpacket(Ptr<Packet> p1)
{
//	if(m_waitqueue.size()<=20)
//	{
//		m_waitqueue.push_back(p);
//		Ptr<Packet> p1 = (m_waitqueue.front ())->Copy ();
		SenderTypeTag tag;		//在nano-routing-entity.h中定义，大小为4字节
		bool i = false;			//邻居节点是否有网关或者路由节点的标志位
		NanoMacHeader macheader;		//macheader大小为8字节
		GetDevice ()->GetMac ()->CheckForNeighborss();
		std::vector<NanoDetail> neighbors = GetDevice ()->GetMac ()->m_neighborss;	//获取邻居节点，类型包括移动性(in_mobility)、index（in_index）、节点id（id)、节点类型，节点类型使用neighbor.type，0代表纳米节点，1代表路由节点，2代表网关节点
		if (neighbors.size () != 0)
		{
			std::vector<NanoDetail> newNeighbors;
			std::vector<NanoDetail>::iterator it,j,temp;
			for (it = neighbors.begin (); it != neighbors.end (); it++)
			{
				if((*it).type==1 || (*it).type == 2)			//如果是路由节点或者网关节点,将数据包发送给路由或网关节点，其它节点不接收
				{
					i = true;
					macheader.SetSource(GetDevice()->GetNode()->GetId());
					macheader.SetDestination(999);
					tag.type = 1;
					p1->AddPacketTag(tag);				//tag是放入packet中的m_packetTagList，不读入数据包长度
					p1->AddHeader(macheader);			//p1的大小为146字节，Packet为102字节，seqTs为12字节，header为24字节，macheader大小为8字节
					//std::cout << p1 << " " << p1->GetSize() << std::endl;
					GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
					Send(p1, 999);
					//m_waitqueue.pop_front ();
					break;
				}
				else
				{
					uint32_t Mobility = (*it).in_mobility;
					if (GetDevice()->Getmobility() < Mobility) 		//将移动性优于本节点的其余纳米节点放入新队列，即候选节点集
					{
						newNeighbors.push_back (*it);
					}
				}
			}

			if (newNeighbors.size () != 0 && i == false)	 		//将数据包转发给移动性优于本节点的邻居节点
			{
				   GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
				   macheader.SetSource(GetDevice ()->GetNode ()->GetId ());
				   macheader.SetDestination(999);
				   tag.type = 2;
				   p1->AddHeader(macheader);
				   p1->AddPacketTag (tag);
				   Send(p1, 999);
				   //m_waitqueue.pop_front ();
			}
			else if(newNeighbors.size () == 0 && i == false)	 		//邻居节点没有移动性大于自己的，选择移动性大于0的邻居节点接收数据包
			{
				GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
				macheader.SetSource(GetDevice ()->GetNode ()->GetId ());
				macheader.SetDestination(999);
				tag.type = 3;
				p1->AddHeader(macheader);
				p1->AddPacketTag (tag);
				Send(p1, 999);
			}
		}
	//}		//周围没有邻居节点，继续进行能量捕获
}

void FSANanoMacEntity::sendtestpacket(Ptr<Packet> p)			//纳米网关节点发送探测数据包
{
	NanoMacHeader header;
	header.SetDestination(999);
	header.SetSource(0);
	p->AddHeader(header);
	m_queue.push_back (p);
	DoSendPacket();
}

void FSANanoMacEntity::Send (Ptr<Packet> p)						//未使用该函数，使用的是下面的带有dst的Send函数
{
  NS_LOG_FUNCTION (this << p);

  NanoMacHeader header;
  uint32_t src = 0;
  uint32_t dst = 999;
  header.SetSource (src);
  header.SetDestination (dst);
  NS_LOG_FUNCTION (this << "mac header" << header);
  p->AddHeader (header);
  m_queue.push_back (p);
  if (m_queue.size () > 0)
	{
	  Simulator::Schedule (Seconds (0.0), &FSANanoMacEntity::DoSendPacket, this);
	}
}

void FSANanoMacEntity::Send (Ptr<Packet> p, uint32_t dst)
{
	m_queue.push_back (p);
	if (m_queue.size () == 1)
	{
		Simulator::Schedule (Seconds (0.0), &FSANanoMacEntity::DoSendPacket, this);
	}
}

void FSANanoMacEntity::forward(Ptr<Packet> p1)				//未使用
{
	//if(m_forwardqueue.size()<=20)
	//{
		//m_forwardqueue.push_back(p);
		//Ptr<Packet> p1 = (m_forwardqueue.front ())->Copy ();
		SenderTypeTag tag;
		NanoMacHeader header;
		bool i = false;			//邻居节点是否有网关或者路由节点的标志位
		GetDevice ()->GetMac ()->CheckForNeighborss();
		std::vector<NanoDetail> neighbors = GetDevice ()->GetMac ()->m_neighborss;
	    if (neighbors.size () != 0)
	    {
	    	GetDevice()->ConsumeEnergySend(GetDevice()->GetTestSize());					//所有数据包转发都需要通过候选节点选择，所以要消耗发送探测数据包的能量

		    std::vector<NanoDetail> newNeighbors;
		    std::vector<NanoDetail>::iterator it, j, temp;
		    for (it = neighbors.begin (); it != neighbors.end (); it++)
			{
		    	GetDevice()->ConsumeEnergyReceive(GetDevice()->GetTestSize());			//节点消耗接收探测数据包的能量

		    	if ((*it).type == 1 || (*it).type == 2)						//如果是路由节点或者网关节点,将数据包发送给路由或网关节点，其它节点不接收
				{
		    		//if(GetDevice()->GetEnergyCapacity() >( GetDevice()->GetPacketSize())*(GetDevice()->GetEnergySendPerByte()))
		    		//{
						i = true;
						header.SetSource(GetDevice()->GetNode()->GetId());
						header.SetDestination(999);
						p1->AddHeader(header);
						GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
						tag.type = 1;
						p1->AddPacketTag(tag);
						Send(p1, 999);
						//m_forwardqueue.pop_front ();
						break;
					//}
				}
				else
				{
					uint32_t Mobility = (*it).in_mobility;
					if (GetDevice()->Getmobility() < Mobility)  		//将移动性优于本节点的其余纳米节点放入新队列，即候选节点集
					{
						newNeighbors.push_back (*it);
					}
				}
			}
		    if (newNeighbors.size() != 0 && i == false)	 		//将数据包转发给移动性优于本节点的邻居节点
			{
				//if(GetDevice()->GetEnergyCapacity() >( GetDevice()->GetPacketSize())*(GetDevice()->GetEnergySendPerByte()))
				//{
					   header.SetDestination(999);
					   header.SetSource(GetDevice ()->GetNode ()->GetId ());
					   p1->AddHeader (header);
					   GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
					   tag.type = 2;
					   p1->AddPacketTag (tag);
					   Send(p1, 999);
					   //m_forwardqueue.pop_front ();
				//}
		    }
			else if (newNeighbors.size() == 0 && i == false)	 		//邻居节点没有移动性大于自己的，选择移动性大于0的邻居节点接收数据包
			{
				GetDevice()->ConsumeEnergySend(GetDevice()->GetPacketSize());				//节点消耗发送数据包的能量
				header.SetSource(GetDevice ()->GetNode ()->GetId ());
				header.SetDestination(999);
				p1->AddHeader(header);
				tag.type = 3;
				p1->AddPacketTag (tag);
				Send(p1, 999);
			}
	    }
	//}			//周围没有邻居节点，继续进行能量捕获
}

void FSANanoMacEntity::Receive (Ptr<Packet> p)
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
