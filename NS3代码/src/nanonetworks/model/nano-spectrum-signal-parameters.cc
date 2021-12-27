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

#include <ns3/log.h>
#include "nano-spectrum-signal-parameters.h"


NS_LOG_COMPONENT_DEFINE ("NanoSpectrumSignalParameters");

namespace ns3 {

	NanoSpectrumSignalParameters::NanoSpectrumSignalParameters ()
	{
	  NS_LOG_FUNCTION (this);
	  m_packet = 0;
	}

	NanoSpectrumSignalParameters::NanoSpectrumSignalParameters (const NanoSpectrumSignalParameters& p) : SpectrumSignalParameters (p)
	{
	  NS_LOG_FUNCTION (this << &p);
	  m_packet = p.m_packet->Copy ();
	  m_duration = p.m_duration;
	  m_pulseDuration = p.m_pulseDuration;
	  m_pulseInterval = p.m_pulseInterval;
	  m_startTime = p.m_startTime;
	}

	Ptr<SpectrumSignalParameters> NanoSpectrumSignalParameters::Copy ()
	{
	  NS_LOG_FUNCTION (this);
	  return Create<NanoSpectrumSignalParameters> (*this);
	}

} // namespace ns3
