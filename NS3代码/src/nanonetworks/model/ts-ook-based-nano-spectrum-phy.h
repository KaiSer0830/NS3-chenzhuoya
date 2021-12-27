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

#ifndef TS_OOK_BASED_NANO_SPECTRUM_PHY_H
#define TS_OOK_BASED_NANO_SPECTRUM_PHY_H


#include <ns3/spectrum-value.h>
#include <ns3/mobility-model.h>
#include <ns3/packet.h>
#include <ns3/nstime.h>
#include <ns3/spectrum-phy.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-signal-parameters.h>
#include <ns3/spectrum-interference.h>
#include <ns3/generic-phy.h>
#include <ns3/lte-spectrum-phy.h>
#include "nano-spectrum-phy.h"
#include <vector>
#include <map>
#include "ns3/callback.h"
#include "ns3/traced-callback.h"


namespace ns3 {

	#define PHY_BITS_HOVERHEAD 16

	struct NanoSpectrumSignalParameters;
	/**
	 * \ingroup nanonetworks
	 *
	 * The TsOokBasedNanoSpectrumPhy models the physical layer of a nanonetwork
	 * based on the TS-OOK modulation scheme
	 */
	class TsOokBasedNanoSpectrumPhy : public NanoSpectrumPhy
	{

		public:
			TsOokBasedNanoSpectrumPhy ();
			virtual ~TsOokBasedNanoSpectrumPhy ();

			static TypeId GetTypeId (void);
			void DoDispose ();

			virtual bool StartTx (Ptr<Packet> p);
			//virtual bool StartTxack (Ptr<Packet> p,uint32_t prio);
			//virtual bool StartTxtest (Ptr<Packet> p);
			virtual void StartRx (Ptr<SpectrumSignalParameters> params);
			virtual void EndRx (Ptr<SpectrumSignalParameters> params);
			void EndTx ();

			void SetPulseDuration (Time d);
			void SetPulseInterval (Time d);
			Time GetPulseDuration (void);
			Time GetPulseInterval (void);

			virtual double GetTxDuration (Ptr<Packet> p);

			bool CheckCollision (Ptr<NanoSpectrumSignalParameters> params);

		private:
			Time m_pulseDuration;			//脉冲周期,主函数中直接设置传入
			Time m_pulseInterval;			//脉冲时隙,主函数中直接设置传入

			struct receivingpacket
			{
			  Ptr<NanoSpectrumSignalParameters> params;
			  bool correct;			//代表NanoSpectrumSignalParameters对象传输是否有冲突，为true代表对象无冲突，为false代表有冲突
			};
			std::list<receivingpacket> *m_receivingpackets;

			typedef void (* OutPhyTxCallback) (int, int);
			typedef void (* OutPhyCollCallback) (int, int);
			TracedCallback<int, int> m_outPHYTX;
			TracedCallback<int, int> m_outPHYCOLL;
	};
}

#endif /* TS_OOK_BASED_NANO_SPECTRUM_PHY_H */
