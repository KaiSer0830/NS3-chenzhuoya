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

#include "nano-spectrum-value-helper.h"
#include <cmath>

namespace ns3 {

Ptr<SpectrumModel> NanoSpectrumModel;

class static_NanoSpectrumModel_initializer
{
	public:
		static_NanoSpectrumModel_initializer ()
		{
			/*
			 * BW = 10 GHz at 1 THz, sub channels of 100 MHz
			 */
			std::vector<double> freqs;
			for (int i = 0; i < 100; ++i)
			{
				double centralFrequencyOfPRB = 990 + (i * 0.1);
				freqs.push_back (centralFrequencyOfPRB * 1e9);
			}
			NanoSpectrumModel = Create<SpectrumModel> (freqs);
		}

	} static_NanoSpectrumModel_initializer_instance;


	Ptr<SpectrumValue> NanoSpectrumValueHelper::CreateTxPowerSpectralDensity (double powerTx)
	{
		Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (NanoSpectrumModel);
		powerTx = pow (10., (powerTx - 30) / 10);
		double txPowerDensity = (powerTx / txPsd->GetSpectrumModel ()->GetNumBands ()) / 180000;
		(*txPsd) = txPowerDensity;
		return txPsd;
	}


	Ptr<SpectrumValue> NanoSpectrumValueHelper::CreateNoisePowerSpectralDensity (void)
	{
		Ptr<SpectrumValue> txPsd = Create <SpectrumValue> (NanoSpectrumModel);
		double noise_db = 2.5 + (-174) + (10. * log10 (180000)) - 30;
		double noisePowerDensity = (pow (10.,noise_db / 10)) / 180000;
		(*txPsd) = noisePowerDensity;
		return txPsd;
	}

} // namespace ns3
