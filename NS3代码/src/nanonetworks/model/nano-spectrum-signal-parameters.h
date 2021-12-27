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

#ifndef NANO_SPECTRUM_SIGNAL_PARAMETERS_H
#define NANO_SPECTRUM_SIGNAL_PARAMETERS_H

#include <ns3/spectrum-signal-parameters.h>
#include <ns3/packet.h>

namespace ns3 {

/**
 * \ingroup Nanonetworks
 *
 * Signal parameters for nanonetworks
 */
struct NanoSpectrumSignalParameters : public SpectrumSignalParameters
{

  // inherited from SpectrumSignalParameters
  virtual Ptr<SpectrumSignalParameters> Copy ();

  /**
   * default constructor
   */
  NanoSpectrumSignalParameters ();

  /**
   * copy constructor
   */
  NanoSpectrumSignalParameters (const NanoSpectrumSignalParameters& p);

  Ptr<Packet> m_packet;
  Time m_duration;
  Time m_pulseDuration;
  Time m_pulseInterval;
  Time m_startTime;
};

}  // namespace ns3


#endif /* NANO_SPECTRUM_SIGNAL_PARAMETERS_H */
