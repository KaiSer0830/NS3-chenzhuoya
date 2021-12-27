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
//#define NS_LOG_APPEND_CONTEXT std::clog << Simulator::Now ().GetSeconds () << " [node " << GetDevice ()->GetNode ()->GetId () << "]";
#define NS_LOG_APPEND_CONTEXT  \
  if (GetDevice () && GetDevice ()->GetNode ()) {  \
      std::clog << Simulator::Now ().GetSeconds ()  \
                << " [node " << GetDevice ()->GetNode ()->GetId () << "] "; }


#include <ns3/waveform-generator.h>
#include <ns3/object-factory.h>
#include <ns3/log.h>
#include <ns3/simulator.h>
#include "ns3/spectrum-error-model.h"
#include "ts-ook-based-nano-spectrum-phy.h"
#include "simple-nano-device.h"
#include "nano-spectrum-channel.h"
#include <math.h>
#include "nano-spectrum-signal-parameters.h"


NS_LOG_COMPONENT_DEFINE ("TsOokBasedNanoSpectrumPhy");

namespace ns3 {


NS_OBJECT_ENSURE_REGISTERED (TsOokBasedNanoSpectrumPhy);

TsOokBasedNanoSpectrumPhy::TsOokBasedNanoSpectrumPhy ()
{
	Initialize ();
	SetState (TsOokBasedNanoSpectrumPhy::IDLE);
	m_receivingpackets = new std::list<receivingpacket>;
}

TypeId TsOokBasedNanoSpectrumPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::TsOokBasedNanoSpectrumPhy")
    .SetParent<NanoSpectrumPhy> ()
    .AddTraceSource ("outPHYTX",  "outPHYTX",  MakeTraceSourceAccessor (&TsOokBasedNanoSpectrumPhy::m_outPHYTX),
                     "ns3::TsOokBasedNanoSpectrumPhy::OutPhyTxCallback")
    .AddTraceSource ("outPHYCOLL",  "outPHYCOLL",  MakeTraceSourceAccessor (&TsOokBasedNanoSpectrumPhy::m_outPHYCOLL),
                     "ns3::TsOokBasedNanoSpectrumPhy::OutPhyCollCallback");
  return tid;
}

std::ostream& operator<< (std::ostream& os, TsOokBasedNanoSpectrumPhy::State s)
{
  switch (s)
    {
    case TsOokBasedNanoSpectrumPhy::IDLE:
      os << "IDLE";
      break;
    case TsOokBasedNanoSpectrumPhy::RX:
      os << "RX";
      break;
    case TsOokBasedNanoSpectrumPhy::TX:
      os << "TX";
      break;
    case TsOokBasedNanoSpectrumPhy::TX_RX:
      os << "TX_RX";
      break;
    default:
      os << "UNKNOWN";
      break;
    }
  return os;
}

TsOokBasedNanoSpectrumPhy::~TsOokBasedNanoSpectrumPhy ()
{
}

void TsOokBasedNanoSpectrumPhy::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  m_receivingpackets->clear ();
  delete m_receivingpackets;
  NanoSpectrumPhy::DoDispose ();
}


bool TsOokBasedNanoSpectrumPhy::StartTx (Ptr<Packet> p)					//从FSANanoMacEntity::DoSendPacket中的phy->StartTx (p)过来
{
  NS_LOG_FUNCTION (this << p);

  Ptr<NanoSpectrumSignalParameters> txParams = Create<NanoSpectrumSignalParameters> ();
  txParams->duration = m_pulseDuration;						//后面重新赋值，貌似这句没有意义
  txParams->psd = GetTxPowerSpectralDensity ();					//传输功率谱密度,继承自SpectrumSignalParameters
  txParams->txPhy = GetObject<SpectrumPhy> ();					//SpectrumPhy对象,继承自SpectrumSignalParameters

  double nbPulseIntervals = p->GetSize () * 8 + PHY_BITS_HOVERHEAD - 1;				//数据包中的总时隙数目,数据包大小+头,PHY_BITS_HOVERHEAD代表16位，因为时隙是脉冲信号与脉冲信号之间的时间间隔，所以要在脉冲总数上-1
  double duration = (nbPulseIntervals * m_pulseInterval.GetFemtoSeconds ()) + m_pulseDuration.GetFemtoSeconds ();		//传输总花费时间,为什么m_pulseDuration不用乘以个数，王超超解释是说时间太小，可以忽略不讲

  txParams->m_packet = p;
  txParams->m_duration = FemtoSeconds (duration);		//传输总花费时间
  txParams->m_pulseDuration = m_pulseDuration;			//脉冲周期,主函数中直接设置传入,100fs
  txParams->m_pulseInterval = m_pulseInterval;			//脉冲时隙,主函数中直接设置传入,10ps
  txParams->m_startTime = Simulator::Now ();
  Simulator::Schedule (FemtoSeconds (duration), &TsOokBasedNanoSpectrumPhy::EndTx, this);
  if (GetState () == IDLE)
  {
	  SetState (NanoSpectrumPhy::TX);
  }
  else if (GetState () == RX)
  {
	  SetState (NanoSpectrumPhy::TX_RX);
  }
  Ptr<SimpleNanoDevice> dev = GetDevice ()->GetObject <SimpleNanoDevice>();			//这句没有意义
  m_outPHYTX ((int)GetDevice ()->GetNode ()->GetId (), (int)p->GetUid ());

  Ptr<NanoSpectrumChannel> c = GetChannel ()->GetObject<NanoSpectrumChannel> ();
  c->StartTx (txParams);
  return true;
}

//bool TsOokBasedNanoSpectrumPhy::StartTxack (Ptr<Packet> p,uint32_t prio)
//{
//  NS_LOG_FUNCTION (this << p);
//
//  Ptr<NanoSpectrumSignalParameters> txParams = Create<NanoSpectrumSignalParameters> ();
//  txParams->duration = m_pulseDuration;
//  txParams->psd = GetTxPowerSpectralDensity ();
//  txParams->txPhy = GetObject<SpectrumPhy> ();
//
//  double nbPulseIntervals = p->GetSize () * 8 + PHY_BITS_HOVERHEAD - 1;
//
//  double duration = (nbPulseIntervals * m_pulseInterval.GetFemtoSeconds ()) + m_pulseDuration.GetFemtoSeconds ();
//
//  txParams->m_packet = p;
//  txParams->m_duration = FemtoSeconds (duration);
//  txParams->m_pulseDuration = m_pulseDuration;
//  txParams->m_pulseInterval = m_pulseInterval;
//  txParams->m_startTime = Simulator::Now ();
//
//  Ptr<NanoSpectrumChannel> c = GetChannel ()->GetObject<NanoSpectrumChannel> ();
//
//  Simulator::Schedule (FemtoSeconds (duration), &TsOokBasedNanoSpectrumPhy::EndTx, this);
//  if(GetState()==NanoSpectrumPhy::CCA_BUSY)
//	  return false;
//  else
//  {
//	  Time t=MicroSeconds(16.0)+(prio-1)*MicroSeconds(9);
////	  while(t.IsPositive())
////	  {
////		  if(GetState()==NanoSpectrumPhy::CCA_BUSY)
////		  {
////			  return false;
////		  }
////		  t=t-MicroSeconds(1.0);
////	  }
//	  SetState (NanoSpectrumPhy::CCA_BUSY);
//  }
//  Ptr<SimpleNanoDevice> dev = GetDevice ()->GetObject <SimpleNanoDevice>();
//
//  //m_outPHYTX ((int)p->GetUid (), (int)GetDevice ()->GetNode ()->GetId ());
//
//  c->StartTx (txParams);
//  return true;
//}

//bool TsOokBasedNanoSpectrumPhy::StartTxtest (Ptr<Packet> p)
//{
//  NS_LOG_FUNCTION (this << p);
//
//  Ptr<NanoSpectrumSignalParameters> txParams = Create<NanoSpectrumSignalParameters> ();
//  txParams->duration = m_pulseDuration;
//  txParams->psd = GetTxPowerSpectralDensity ();
//  txParams->txPhy = GetObject<SpectrumPhy> ();
//
//  double nbPulseIntervals = p->GetSize () * 8 + PHY_BITS_HOVERHEAD - 1;
//
//  double duration = (nbPulseIntervals * m_pulseInterval.GetFemtoSeconds ()) + m_pulseDuration.GetFemtoSeconds ();
//
//  txParams->m_packet = p;
//  txParams->m_duration = FemtoSeconds (duration);
//  txParams->m_pulseDuration = m_pulseDuration;
//  txParams->m_pulseInterval = m_pulseInterval;
//  txParams->m_startTime = Simulator::Now ();
//
//  Ptr<NanoSpectrumChannel> c = GetChannel ()->GetObject<NanoSpectrumChannel> ();
//
//  Simulator::Schedule (FemtoSeconds (duration), &TsOokBasedNanoSpectrumPhy::EndTx, this);
//  if (GetState () == IDLE)
//       {
//   	  SetState (NanoSpectrumPhy::TX);
//       }
//     else if (GetState () == RX)
//       {
//   	  SetState (NanoSpectrumPhy::TX_RX);
//       }
//
//  Ptr<SimpleNanoDevice> dev = GetDevice ()->GetObject <SimpleNanoDevice>();
//
//  m_outPHYTX ((int)p->GetUid (), (int)GetDevice ()->GetNode ()->GetId ());
//
//  c->StartTx (txParams);
//  return true;
//}

void TsOokBasedNanoSpectrumPhy::EndTx ()				//
{
	NS_LOG_FUNCTION (this);
	if (m_receivingpackets->size () == 0)
	{
		SetState (NanoSpectrumPhy::IDLE);
	}
	else
	{
		SetState (NanoSpectrumPhy::RX);
	}
}

void TsOokBasedNanoSpectrumPhy::SetPulseDuration (Time d)
{
	NS_LOG_FUNCTION (this << d);
	m_pulseDuration = d;
}

void TsOokBasedNanoSpectrumPhy::SetPulseInterval(Time d) {
	NS_LOG_FUNCTION(this << d);
	m_pulseInterval = d;
}

Time TsOokBasedNanoSpectrumPhy::GetPulseDuration(void) {
	NS_LOG_FUNCTION(this);
	return m_pulseDuration;
}

Time TsOokBasedNanoSpectrumPhy::GetPulseInterval(void) {
	NS_LOG_FUNCTION(this);
	return m_pulseInterval;
}

void TsOokBasedNanoSpectrumPhy::StartRx (Ptr<SpectrumSignalParameters> spectrumParams)				//从NanoSpectrumChannel::StartTx(Ptr<SpectrumSignalParameters> txParams)中过来，接收数据的准备工作5
{
	NS_LOG_FUNCTION(this << spectrumParams << "from " << spectrumParams->txPhy);
	NS_LOG_LOGIC(this << "state: " << GetState () << m_receivingpackets->size ());

	Ptr<NanoSpectrumSignalParameters> params = DynamicCast<NanoSpectrumSignalParameters>(spectrumParams);

	if (params != 0) {
		Time duration = params->m_duration;				//完成接收数据包的时间，与发送数据包的时间一致，一般为+11670100.0fs

		NS_LOG_LOGIC(this << " duration " << duration.GetPicoSeconds ());

		receivingpacket rp;
		rp.params = DynamicCast<NanoSpectrumSignalParameters>(params->Copy());
		rp.correct = true;					//代表NanoSpectrumSignalParameters对象传输是否有冲突，为true代表对象无冲突，为false代表有冲突
		m_receivingpackets->push_back(rp);				//m_receivingpackets存放所有已接收待处理的数据
		if (GetState() == NanoSpectrumPhy::IDLE || GetState() == NanoSpectrumPhy::CCA_BUSY) {
			SetState(NanoSpectrumPhy::RX);
		} else if (GetState() == NanoSpectrumPhy::TX || GetState() == NanoSpectrumPhy::CCA_BUSY) {
			SetState(NanoSpectrumPhy::TX_RX);
		}

		NS_LOG_LOGIC(this << "state: " << GetState () << m_receivingpackets->size ());

		Simulator::Schedule(duration, &TsOokBasedNanoSpectrumPhy::EndRx, this, params);
	}
}

void TsOokBasedNanoSpectrumPhy::EndRx (Ptr<SpectrumSignalParameters> spectrumParams)				//接收数据后进行数据判断
{
	NS_LOG_FUNCTION(this << spectrumParams << "state: " << GetState () << m_receivingpackets->size ());

	Ptr<NanoSpectrumSignalParameters> params = DynamicCast<NanoSpectrumSignalParameters>(spectrumParams);
	Ptr<Packet> p = params->m_packet;

	if (!CheckCollision(params)) {
		NS_LOG_FUNCTION(this << " receiving the packet at PHY");
		GetDevice()->GetObject<SimpleNanoDevice>()->ReceivePacket(p);				//数据不冲突，则进行接收，跳转到OpportunisticNanoRoutingEntity::ReceivePacket (Ptr<Packet> p)
	} else {
		NS_LOG_FUNCTION(this << "PHY collision");
		m_outPHYCOLL((int) GetDevice()->GetNode()->GetId(), (int) p->GetUid());		//数据冲突，则使用log文件进行记录
	}

	if (GetState() == NanoSpectrumPhy::TX_RX && m_receivingpackets->size() == 0) {
		SetState(NanoSpectrumPhy::TX);
	} else if (m_receivingpackets->size() == 0) {
		SetState(NanoSpectrumPhy::IDLE);
	}
	NS_LOG_FUNCTION (this << "state: " << GetState () << m_receivingpackets->size ());
}

bool TsOokBasedNanoSpectrumPhy::CheckCollision (Ptr<NanoSpectrumSignalParameters> params)			//从m_receivingpackets队列中查看是否有NanoSpectrumSignalParameters存在相同的开始传输时间
{
	NS_LOG_FUNCTION(this << "tot pkts: " << m_receivingpackets->size ());

	bool collision = false;
	receivingpacket rp;

	if (m_receivingpackets->size() == 1) {
		NS_LOG_FUNCTION(this << "only 1 pkt" << m_receivingpackets->size ());
		receivingpacket rp = *m_receivingpackets->begin();
		collision = !rp.correct;					//m_receivingpackets只有一个元素时，correct默认为true，collision返回为false
		m_receivingpackets->clear();
		NS_LOG_FUNCTION(this << collision);
		return collision;
	} else {
		NS_LOG_FUNCTION(this << "more pkts" << m_receivingpackets->size ());
		std::list<receivingpacket>::iterator it;
		std::list<receivingpacket> *newvet = new std::list<receivingpacket>;
		for (it = m_receivingpackets->begin(); it != m_receivingpackets->end(); it++) {
			receivingpacket rp = *it;
			NS_LOG_FUNCTION(this << "compare" << rp.params->txPhy << "and" << params->txPhy);
			if (rp.params->txPhy != params->txPhy) {			//从m_receivingpackets中过滤掉自己
				NS_LOG_FUNCTION(this << "add");
				newvet->push_back(rp);			//newvet存放m_receivingpackets队列中除自己以外的NanoSpectrumSignalParameters
			}
		}

		NS_LOG_FUNCTION(this << "new" << newvet->size () << "old" << m_receivingpackets->size ());
		m_receivingpackets->clear();
		delete m_receivingpackets;
		m_receivingpackets = newvet;								//将newvet列表更名为m_receivingpackets列表
		NS_LOG_FUNCTION(this << m_receivingpackets->size ());

		for (it = m_receivingpackets->begin(); it != m_receivingpackets->end(); it++) {
			receivingpacket rp = *it;
			NS_LOG_FUNCTION(this << "check among" << rp.params->txPhy << rp.params->m_pulseInterval << rp.params->m_startTime << "and" << params->txPhy << params->m_pulseInterval << params->m_startTime);

			if (rp.params->m_startTime == params->m_startTime) {			//如果m_receivingpackets列表中存在与params相同的开始传输时间，则代表有冲突
				NS_LOG_FUNCTION(this << "collision");
				collision = true;
				rp.correct = false;
			} else {
				Time timeDistance = rp.params->m_startTime - params->m_startTime;
				NS_LOG_FUNCTION(this
						<< timeDistance);
				if (timeDistance.GetFemtoSeconds() % rp.params->m_pulseInterval.GetFemtoSeconds() == 0) {		//如果m_receivingpackets列表中存在传输开始时间与params的传输开始时间之差是脉冲间隙的整数倍，则代表有冲突
					NS_LOG_FUNCTION(this << "collision");
					collision = true;
					rp.correct = false;
				}
			}
		}
		return collision;
	}
}


double TsOokBasedNanoSpectrumPhy::GetTxDuration (Ptr<Packet> p)
{
	double nbPulseIntervals = p->GetSize () * 8 + PHY_BITS_HOVERHEAD - 1;
	double duration = (nbPulseIntervals * m_pulseInterval.GetFemtoSeconds ()) + m_pulseDuration.GetFemtoSeconds ();
	return duration;
}

} // namespace ns3
