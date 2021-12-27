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

#ifndef SIMPLE_NANO_DEVICE_H
#define SIMPLE_NANO_DEVICE_H

#include <string.h>
#include <ns3/node.h>
#include <ns3/address.h>
#include <ns3/net-device.h>
#include <ns3/callback.h>
#include <ns3/packet.h>
#include <ns3/traced-callback.h>
#include <ns3/ptr.h>
#include <list>


namespace ns3 {

	class NanoSpectrumPhy;
	class NanoMacEntity;
	class NanoRoutingEntity;
	class MessageProcessUnit;

	/**
	 * \ingroup nanonetworks
	 *
	 * This class provides a template implementation
	 * of a a generic nano device
	 */
	class SimpleNanoDevice : public NetDevice
	{
		public:
			static TypeId GetTypeId (void);

			SimpleNanoDevice ();
			virtual ~SimpleNanoDevice ();

			virtual void DoDispose (void);

			// inherited from NetDevice
			virtual void SetIfIndex (const uint32_t index);
			virtual uint32_t GetIfIndex (void) const;
			virtual Ptr<Channel> GetChannel (void) const;
			virtual bool SetMtu (const uint16_t mtu);
			virtual uint16_t GetMtu (void) const;
			virtual void SetAddress (Address address);
			virtual Address GetAddress (void) const;
			virtual bool IsLinkUp (void) const;
			virtual void AddLinkChangeCallback (Callback<void> callback);
			virtual bool IsBroadcast (void) const;
			virtual Address GetBroadcast (void) const;
			virtual bool IsMulticast (void) const;
			virtual bool IsPointToPoint (void) const;
			virtual bool IsBridge (void) const;
			virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
			virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
			virtual Ptr<Node> GetNode (void) const;
			virtual void SetNode (Ptr<Node> node);
			virtual bool NeedsArp (void) const;
			virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
			virtual Address GetMulticast (Ipv4Address addr) const;
			virtual Address GetMulticast (Ipv6Address addr) const;
			virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
			virtual bool SupportsSendFrom (void) const;


			/**
			* Set the Phy object which is attached to this device.
			* @param phy the Phy object embedded within this device.
			*/
			void SetPhy (Ptr<NanoSpectrumPhy> phy);

			/**
			* @return a reference to the PHY object embedded in this NetDevice.
			*/
			Ptr<NanoSpectrumPhy> GetPhy () const;

			/**
			* Set the
			* @param mac
			*/
			void SetMac (Ptr<NanoMacEntity> mac);

			/**
			* @return a reference to
			*/
			Ptr<NanoMacEntity> GetMac () const;

			/**
			* Set the
			* @param routing
			*/
			void SetL3 (Ptr<NanoRoutingEntity> l3);

			/**
			* @return a reference to
			*/
			Ptr<NanoRoutingEntity> GetL3 () const;


			void SendPacket (Ptr<Packet> p);
			void SendtestPacket (Ptr<Packet> p);					//flow-guided,发送探测数据包
			void SendackPacket (Ptr<Packet> p,uint32_t dst);		//flow-guided,发送响应数据包
			void ReceivePacket (Ptr<Packet> p);

			void SetMessageProcessUnit (Ptr<MessageProcessUnit> mpu);
			Ptr<MessageProcessUnit> GetMessageProcessUnit (void);

			void SetEnergyCapacity(double energy);					//flow-guided,设置存储能量大小
			double GetEnergyCapacity() const;							//flow-guided,获取存储能量大小
			void SetMaxEnergy(double maxenergy);						//flow-guided,设置最大存储能量大小
			void SetHarvestEnergyInterTime(double t);					//flow-guided,设置能量捕获时隙
			void SetHarEnergySpeed(int speed);						//flow-guided,设置能量捕获速率
			void HarvestEnergy();//reduceenery是一个随机数，用以模拟接收能量随机的情况。		//flow-guided
			//在应用层reducenergy进行随机化，范围有待确定。目前设置为0.表示接受能量一定。
			//单位是speed
			double ConsumeEnergySend(double packetsize);					//flow-guided
			double ConsumeEnergyReceive(double packetsize);				//flow-guided
			void SetEnergySendPerByte(double energysendperbyte);			//flow-guided,设置发送能耗/字节
			void SetEnergyReceivePerByte(double energyreceiveperbyte);	//flow-guided,设置接收能耗/字节
			double GetEnergySendPerByte();								//flow-guided,获取发送能耗/字节
			double GetEnergyReceivePerByte();								//flow-guided,获取接收能耗/字节
			void SetPacketSize(double packetsize);

			double GetPacketSize();
			int GetTestSize();						//flow-guided,获取探测数据包大小
			void SetTestSize(double testsize);		//flow-guided,设置探测数据包大小
			void SetParameter(double parameter1,double parameter2);
			double GetM();
			void SetU(double u);

			void SetaverageIndex(double averageIndex);					//flow-guided,设置平均Index值
			double GetaverageIndex();										//flow-guided,获取平均Index值
			void Setmobility(double Mobile);
			double Getmobility();

			enum NodeType
			{
				NanoNode,NanoRouter,NanoInterface
			};
			NodeType m_type;			//节点类型,0代表纳米节点，1代表路由节点，2代表网关节点

		private:
			Ptr<Node> m_node;
			uint32_t m_ifIndex;
			Ptr<NanoSpectrumPhy> m_phy;
			Ptr<NanoMacEntity> m_mac;
			Ptr<NanoRoutingEntity> m_l3;
			Ptr<MessageProcessUnit> m_mpu;
			uint32_t m_randv;				//flow-guided,使用m_randv变量来作为srand()不同的种子，从而产生不同能量捕获速度的随机值

		public:
			//int m_seed;
			//int m_nodes;
			//int m_apps;
			double m_txrange;
			//int m_gates;
			//int m_routers;

			uint32_t index;				//flow-guided,节点index值
			double averageindex;			//flow-guided,节点的周围平均index值
			double mobile;				//flow-guided,节点相对移动性

			double m_energy;					//flow-guided,节点当前能量
			double m_maxenergy;					//flow-guided,节点最大存储能量
			double m_harenergyintertime;			//flow-guided,节点能量捕获时隙
			int m_harenergyspeed;					//flow-guided,节点能量捕获速度
			double m_EnergySendPerByte;			//flow-guided,发送能耗/字节
			double m_EnergyReceivePerByte;		//flow-guided,接收能耗/字节
			double m_PacketSize;					//flow-guided,数据包大小
			double m_TestSize;					//flow-guided,探测数据包大小

			double M;					//flow-guided,候选节点转发的优先级
			double U,parameter1,parameter2;				//计算候选节点转发优先级的系统参数
			typedef void (* OutPhyTxCallback1) (double, int);
			TracedCallback<int, double, double> m_outEnergy;
	};	//SimpleNanoDevice
} 		//namespace ns3

#endif /* SIMPLE_NANO_DEVICE_H */
