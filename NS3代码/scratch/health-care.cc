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
 *
 *
 * run:
 *
 * ./waf --run "scratch/health-care --seed=1 --nbNanoNodes=200 --nbNanoRouters=50 --nbNanoGateways=1 --txRangeNanoNodes=0.01 --macType=1 --l3Type=1
 */

#include "ns3/core-module.h"                        //build/ns3
#include "ns3/network-module.h"                     //build/ns3
#include "ns3/applications-module.h"                //build/ns3
#include "ns3/mobility-module.h"                    //build/ns3
#include "ns3/config-store-module.h"                //build/ns3
#include "ns3/internet-module.h"                    //build/ns3
#include <iostream>
#include "ns3/global-route-manager.h"
#include "ns3/nano-mac-entity.h"
#include "ns3/nano-mac-queue.h"
#include "ns3/nano-spectrum-channel.h"
#include "ns3/nano-spectrum-phy.h"
#include "ns3/nano-spectrum-signal-parameters.h"
#include "ns3/nano-helper.h"
#include "ns3/nano-spectrum-value-helper.h"
#include "ns3/simple-nano-device.h"
#include "ns3/nanointerface-nano-device.h"
#include "ns3/nanorouter-nano-device.h"
#include "ns3/nanonode-nano-device.h"
#include "ns3/backoff-based-nano-mac-entity.h"
#include "ns3/seq-ts-header.h"
#include "ns3/ts-ook-based-nano-spectrum-phy.h"
#include "ns3/mobility-model.h"
#include "ns3/message-process-unit.h"
#include "ns3/transparent-nano-mac-entity.h"
#include "ns3/random-nano-routing-entity.h"
#include "ns3/flooding-nano-routing-entity.h"
#include"ns3/fsa-nano-mac-entity.h"
#include "ns3/opportunistic-nano-routing-entity.h"

using namespace std;

//日志，声明了一个叫health-care的日志组件，通过引用health-care这个名字的操作，可以实现打开或者关闭控制台日志的输出,全文未使用
NS_LOG_COMPONENT_DEFINE ("health-care");

using namespace ns3;



void Run(int nbNanoNodes,int nbNanoRouters,int nbNanoGateways, double txRangeNanoNodes, double txRangeNanoRouter, double txRangeNanoGateway, int macType, int l3Type, int seed, std::string logFile);

void PrintTXEvents(Ptr<OutputStreamWrapper> stream, int, int);
void PrintRXEvents(Ptr<OutputStreamWrapper> stream, int, int, int, int, double);
void PrintPHYTXEvents(Ptr<OutputStreamWrapper> stream, int, int);
void PrintPHYCOLLEvents(Ptr<OutputStreamWrapper> stream, int, int);
void PrintENERGYEvents(Ptr<OutputStreamWrapper> stream, int, double, double);
void PrintSimulationTime(double duration);
static void PrintPosition(std::ostream *os, std::string foo, NetDeviceContainer devs);
void PrintMemoryUsage(void);
string itos(int i);


int main(int argc, char *argv[]) {
	int nbNanoNodes = 5;
	int nbNanoRouters = 50;
	int nbNanoGateways = 1;
	double txRangeNanoNodes = 0.01;			//纳米节点传输范围0.01m
	double txRangeNanoRouter = 0.02;			//纳米路由节点传输范围0.02m
	double txRangeNanoGateway = 0.02;			//纳米网关节点传输范围0.02m
	int macType = 3;							//Mac协议类型
	int l3Type = 3;							//路由协议类型
	int seed = 1;

	//仿真一般是为了收集各种不同条件下的数据，常常需要改变一些变量。NS-3提供了Command Line参数接口，可以在运行时对脚本中的变量进行设置，免去了每次更改变量后要重新编译和构建脚本的麻烦
	//使用CommandLine::AddValue添加自己的变量，通过钩挂自己的变量将其与命令行相关联，使之成为CommandLine可以使用的参数,格式为（属性名称，属性说明，变量）
	//使用方法./waf --run "scratch/health-care --macType=2"
	CommandLine cmd;
	cmd.AddValue("seed", "seed", seed);
	cmd.AddValue("nbNanoNodes", "nbNanoNodes", nbNanoNodes);
	cmd.AddValue("nbNanoRouters", "nbNanoRouters", nbNanoRouters);
	cmd.AddValue("nbNanoGateways", "nbNanoGateways", nbNanoGateways);
	cmd.AddValue("txRangeNanoNodes", "txRangeNanoNodes", txRangeNanoNodes);
	cmd.AddValue("txRangeNanoRouter", "txRangeNanoRouter", txRangeNanoRouter);
	cmd.AddValue("txRangeNanoGateway", "txRangeNanoGateway", txRangeNanoGateway);
	cmd.AddValue("macType", "macType", macType);
	cmd.AddValue("l3Type", "l3Type", l3Type);
	cmd.Parse(argc, argv);

	Time::SetResolution(Time::FS);
	int nanoNodes[6] = {5, 10, 15, 20, 25, 30};
	for (int x = 1; x < 7; x++) {
		std::string logFile = "nanonodeposition";
		std::string num = itos(x);
		const char* p = num.c_str();
		logFile.append(p);
		logFile.append(".log");
		std::cout << logFile << std::endl;
		Run(nanoNodes[x - 1], nbNanoRouters, nbNanoGateways, txRangeNanoNodes, txRangeNanoRouter, txRangeNanoGateway, macType, l3Type, seed, logFile);
	}
	return 0;
}

string itos(int i) {		//将int转换成string
    std::stringstream s;
    s << i;
    return s.str();
}

void Run(int nbNanoNodes, int nbNanoRouters,int nbNanoGateways, double txRangeNanoNodes, double txRangeNanoRouter,double txRangeNanoGateway, int macType, int l3Type, int seed, std::string logFile)
{
	std::cout << "START SIMULATION WITH: "
			"\n\t nbNanoNodes " << nbNanoNodes <<
			"\n\t nbNanoRouters " << nbNanoRouters <<
			"\n\t nbNanoGateways " << nbNanoGateways <<
			"\n\t txRangeNanoNodes " << txRangeNanoNodes <<
			"\n\t txRangeNanoRouter "<< txRangeNanoRouter <<
			"\n\t txRangeNanoGateway "<< txRangeNanoGateway <<
			"\n\t macType [1->Transparent; 2->BackoffBased;3->Fsa] "<< macType <<
			"\n\t l3Type [1->Flooding; 2->Random Routing; 3->Opportunistic Routing] "<< l3Type <<
			"\n\t seed " << seed << std::endl;

	//timers,设置最小单元时间FS,模拟周期5s

	double duration = 5;

	//layout details,设置动脉大小2*10^-4m^3
	double xrange = 0.5;		//0.5m
	double yrange = 0.02;
	double zrange = 0.02;		//移动模型中已经设置了长宽高，这里定义变量只是为了设置网关与路由的初始位置

	//physical details,物理层信息
	double pulseEnergy = 100e-12;		//脉冲能量100pJ,能量1J = 1*10^3mJ = 1*10^6uJ = 1*10^9nJ = 1*10^12pJ = 1*10^15fJ
	double pulseDuration = 100;			//脉冲周期100fs,时间1s = 10^3ms = 10^6us = 10^9ns = 10^12ps = 10^15fs
	double pulseInterval = 10000;		//脉冲间隙10ps
	double powerTransmission = pulseEnergy / pulseDuration;			//传输功率,用于设置m_txPower和创建功率谱密度函数,在文件nano-spctrum-phy.cc

	//random variable generation,利用系统时间来改变系统的种子值
	srand(time(NULL));
	SeedManager::SetSeed(rand());			//使得每次调用rand函数生成的伪随机数序列不同，从而实现真正意义上的“随机”
	Ptr<UniformRandomVariable> random = CreateObject<UniformRandomVariable> ();			//类UniformRandomVariable从RandomVariableStream继承

	//helper definition
	NanoHelper nano;
	//nano.EnableLogComponents ();

	// Tracing
	AsciiTraceHelper asciiTraceHelper;
	std::stringstream file_outTX_s;
	file_outTX_s << "RES_TX" << "_N_" << nbNanoNodes <<"_R_" << nbNanoRouters << "_G_" << nbNanoGateways <<
			"_nTxRange_" << txRangeNanoNodes<<"_GTxRange_" << txRangeNanoGateway << "_macType_" << macType << "_l3Type_" << l3Type << "_seed_" << seed;
	std::stringstream file_outRX_s;
	file_outRX_s << "RES_RX" << "_N_" << nbNanoNodes<<"_R_" << nbNanoRouters  <<  "_G_" << nbNanoGateways <<
			"_nTxRange_" << txRangeNanoNodes <<"_GTxRange_" << txRangeNanoGateway<< "_macType_" << macType << "_l3Type_" << l3Type << "_seed_" << seed;
	std::stringstream file_outCorrectRX_s;
	std::stringstream file_outPHYTX_s;
	file_outPHYTX_s << "RES_PHYTX" << "_N_" << nbNanoNodes<<"_R_" << nbNanoRouters  <<  "_G_" << nbNanoGateways <<
			"_nTxRange_" << txRangeNanoNodes <<"_GTxRange_" << txRangeNanoGateway<< "_macType_" << macType << "_l3Type_" << l3Type << "_seed_" << seed;
	std::stringstream file_outPHYCOLL_s;
	file_outPHYCOLL_s << "RES_PHYCOLL" << "_N_" << nbNanoNodes <<"_R_" << nbNanoRouters <<  "_G_" << nbNanoGateways <<
			"_nTxRange_" << txRangeNanoNodes <<"_GTxRange_" << txRangeNanoGateway<< "_macType_" << macType << "_l3Type_" << l3Type << "_seed_" << seed;
	std::stringstream file_outENERGY_s;
	file_outENERGY_s << "RES_ENERGY" << "_N_" << nbNanoNodes <<"_R_" << nbNanoRouters <<  "_G_" << nbNanoGateways <<
			"_nTxRange_" << txRangeNanoNodes <<"_GTxRange_" << txRangeNanoGateway<< "_macType_" << macType << "_l3Type_" << l3Type << "_seed_" << seed;

	//streamstring在调用str()时，会返回临时的string对象。而因为是临时的对象，所以它在整个表达式结束后将会被析构。如果需要进一步操作string对象，先把其值赋给一个string变量后再操作。
	std::string file_outTX = file_outTX_s.str();
	std::string file_outRX = file_outRX_s.str();
	std::string file_outPHYTX = file_outPHYTX_s.str();
	std::string file_outPHYCOLL = file_outPHYCOLL_s.str();
	std::string file_outENERGY = file_outENERGY_s.str();

	Ptr<OutputStreamWrapper> streamTX = asciiTraceHelper.CreateFileStream(file_outTX);
	Ptr<OutputStreamWrapper> streamRX = asciiTraceHelper.CreateFileStream(file_outRX);
	Ptr<OutputStreamWrapper> streamPHYTX = asciiTraceHelper.CreateFileStream(file_outPHYTX);
	Ptr<OutputStreamWrapper> streamPHYCOLL = asciiTraceHelper.CreateFileStream(file_outPHYCOLL);
	Ptr<OutputStreamWrapper> streamENERGY = asciiTraceHelper.CreateFileStream(file_outENERGY);

	//network definition
	NodeContainer n_routers;
	NodeContainer n_gateways;
	NodeContainer n_nodes;				//声明了一个名为”n_nodes”的NodeContainer对象
	NetDeviceContainer d_gateways;		//使用一个NetDeviceContainer对象来存放需要所有被创建的NetDevice对象
	NetDeviceContainer d_nodes;
	NetDeviceContainer d_routers;

	n_gateways.Create(nbNanoGateways);
	d_gateways = nano.Install(n_gateways, NanoHelper::nanointerface);
	n_routers.Create(nbNanoRouters);
	d_routers = nano.Install(n_routers, NanoHelper::nanorouter);
	n_nodes.Create(nbNanoNodes);		//调用了n_nodes对象的Create()方法创建了nbNanoNodes个节点。
	d_nodes = nano.Install(n_nodes, NanoHelper::nanonode);


	//mobility
//	MobilityHelper mobility;
//	//初始位置分布器
//	mobility.SetMobilityModel ("ns3::RandomWaypointMobilityModel",
//			"Speed",StringValue ("ns3::UniformRandomVariable[Min=0.2|Max=0.2]"),
//			"Pause",StringValue ("ns3::ConstantRandomVariable[Constant=0.01]"));
//	mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
//			"X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.5]"),//RandomVariableValue (UniformVariable (0, xrange)),
//			"Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.02]"),//RandomVariableValue (UniformVariable (0, yrange)),
//			"Z", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.02]"));//RandomVariableValue (UniformVariable (0, zrange)));

	MobilityHelper mobility;
		//移动轨迹模型
		mobility.SetMobilityModel ("ns3::GaussMarkovMobilityModel",
				//设置移动区域的界限
				"Bounds", BoxValue (Box (0, xrange, 0, yrange, 0, zrange)),
				//设置每一次移动的间隔
				"TimeStep", TimeValue (Seconds (0.001)),
				//设置高斯-马尔可夫模型中可调参数的常数
				"Alpha", DoubleValue (0),
				//用于分配平均速度的随机变量
				"MeanVelocity", StringValue ("ns3::UniformRandomVariable[Min=0.2|Max=0.2]"),
				//用于分配平均方向的随机变量
				"MeanDirection", StringValue ("ns3::UniformRandomVariable[Min=0|Max=0]"),
				//用于分配平均Pitch的随机变量
				"MeanPitch", StringValue ("ns3::UniformRandomVariable[Min=0.05|Max=0.05]"),
				//用于计算下一个速度值的高斯随机变量
				"NormalVelocity", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.0|Bound=0.0]"),
				//用于计算下一个方向值的高斯随机变量
				"NormalDirection", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.2|Bound=0.4]"),
				//用于计算下一个Pitch值的高斯随机变量
				"NormalPitch", StringValue ("ns3::NormalRandomVariable[Mean=0.0|Variance=0.2|Bound=0.4]"));
		//初始位置分布器
		mobility.SetPositionAllocator ("ns3::RandomBoxPositionAllocator",
				"X", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.5]"),//RandomVariableValue (UniformVariable (0, xrange)),
				"Y", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.02]"),//RandomVariableValue (UniformVariable (0, yrange)),
				"Z", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=0.02]"));//RandomVariableValue (UniformVariable (0, zrange)));
	mobility.Install (n_nodes);

	//std::string logFile = logFile;
	std::ofstream os;
	os.open(logFile.c_str());			//c_str是string类的一个函数，可以把string类型变量转换成char*变量,open()要求的是一个char*字符串
	Simulator::Schedule(Seconds(0.001), &PrintPosition, &os, logFile, d_nodes);

	//protocol stack
	for (int x = 0; x < nbNanoGateways; x++) {
		Ptr<NanoInterfaceDevice> interface = d_gateways.Get(x)->GetObject<NanoInterfaceDevice> ();
		Ptr<ConstantPositionMobilityModel> m = CreateObject<ConstantPositionMobilityModel> ();				//网关节点是固定位置移动模型

		m->SetPosition(Vector(xrange/2, yrange/2, 0));				//纳米网关节点的安装位置在动脉的中心点

		nano.AddMobility(interface->GetPhy(), m);

		Ptr<NanoRoutingEntity> routing;
		if (l3Type == 1) {
			Ptr<FloodingNanoRoutingEntity> routing1 = CreateObject<FloodingNanoRoutingEntity> ();
			routing = routing1;
		} else if (l3Type == 2) {
			Ptr<RandomNanoRoutingEntity> routing2 = CreateObject<RandomNanoRoutingEntity> ();
			routing = routing2;
		} else {
			Ptr<OpportunisticNanoRoutingEntity> routing3 = CreateObject<OpportunisticNanoRoutingEntity> ();
			routing = routing3;
		}
		routing->SetDevice(interface);
		interface->SetL3(routing);

		Ptr<NanoMacEntity> mac;
		if (macType == 1) {
			Ptr<TransparentNanoMacEntity> mac1 = CreateObject<TransparentNanoMacEntity> ();
			mac = mac1;
		} else if (macType == 2) {
			Ptr<BackoffBasedNanoMacEntity> mac2 = CreateObject<BackoffBasedNanoMacEntity> ();
			mac = mac2;
		} else {
			Ptr<FSANanoMacEntity> mac3 = CreateObject<FSANanoMacEntity> ();
			mac = mac3;
		}
		mac->SetDevice(interface);
		interface->SetMac(mac);

		interface->GetPhy()->SetTxPower(powerTransmission);				//传输功率=脉冲能量/脉冲间隙
		interface->GetPhy()->SetTransmissionRange(txRangeNanoGateway);
		interface->GetPhy()->GetObject<TsOokBasedNanoSpectrumPhy> ()->SetPulseDuration(FemtoSeconds(pulseDuration));				//设置物理层协议为TsOokBasedNanoSpectrumPhy
		interface->GetPhy()->GetObject<TsOokBasedNanoSpectrumPhy> ()->SetPulseInterval(FemtoSeconds(pulseInterval));

		interface->GetPhy()->TraceConnectWithoutContext("outPHYTX", MakeBoundCallback(&PrintPHYTXEvents, streamPHYTX));				//outPHYTX与outPHYCOLL底层代码在ts-ook-based-nano-spectrum-phy.cc文件
		interface->GetPhy()->TraceConnectWithoutContext("outPHYCOLL", MakeBoundCallback(&PrintPHYCOLLEvents, streamPHYCOLL));
		interface->TraceConnectWithoutContext("outPHYTX1", MakeBoundCallback(&PrintENERGYEvents, streamENERGY));					//outEnergy底层代码在simple-nano-device.cc文件

		double testSize = 50;					//探测数据包大小50
		interface->SetTestSize(testSize);		//网关节点设置节点传输探测数据包的大小
	}

	for (int x = 0; x < nbNanoRouters; x++) {
		Ptr<NanoRouterDevice> router = d_routers.Get(x)->GetObject<NanoRouterDevice> ();
		Ptr<ConstantPositionMobilityModel> m = CreateObject<ConstantPositionMobilityModel> ();				//路由节点是固定位置移动模型
		if(x < nbNanoRouters / 2) {
			m->SetPosition(Vector(x * (xrange / nbNanoRouters), yrange/2, 0.0));				//在空间前半个部分从x=0开始依次平均间隔放置
		} else {
			m->SetPosition(Vector((x + 1) * (xrange / nbNanoRouters), yrange/2, 0.0));				//在空间后半个部分需要加1是因为最中心点放置了网关节点
		}
		nano.AddMobility(router->GetPhy(), m);

		Ptr<NanoRoutingEntity> routing;
		if (l3Type == 1) {
			Ptr<FloodingNanoRoutingEntity> routing1 = CreateObject<FloodingNanoRoutingEntity> ();
			routing = routing1;
		} else if (l3Type == 2) {
			Ptr<RandomNanoRoutingEntity> routing2 = CreateObject<RandomNanoRoutingEntity> ();
			routing = routing2;
		} else {
			Ptr<OpportunisticNanoRoutingEntity> routing3 = CreateObject<OpportunisticNanoRoutingEntity> ();
			routing = routing3;
		}

		routing->SetDevice(router);
		router->SetL3(routing);

		Ptr<NanoMacEntity> mac;
		if (macType == 1) {
			Ptr<TransparentNanoMacEntity> mac1 = CreateObject<TransparentNanoMacEntity> ();
			mac = mac1;
		} else if (macType == 2) {
			Ptr<BackoffBasedNanoMacEntity> mac2 = CreateObject<BackoffBasedNanoMacEntity> ();
			mac = mac2;
		} else {
			Ptr<FSANanoMacEntity> mac3 = CreateObject<FSANanoMacEntity> ();
			mac = mac3;
		}
		mac->SetDevice(router);
		router->SetMac(mac);

		router->GetPhy()->SetTxPower(powerTransmission);
		router->GetPhy()->SetTransmissionRange(txRangeNanoRouter);
		router->GetPhy()->GetObject<TsOokBasedNanoSpectrumPhy> ()->SetPulseDuration(FemtoSeconds(pulseDuration));				//设置物理层协议为TsOokBasedNanoSpectrumPhy
		router->GetPhy()->GetObject<TsOokBasedNanoSpectrumPhy> ()->SetPulseInterval(FemtoSeconds(pulseInterval));

		router->GetPhy()->TraceConnectWithoutContext("outPHYTX", MakeBoundCallback(&PrintPHYTXEvents, streamPHYTX));			//outPHYTX与outPHYCOLL底层代码在ts-ook-based-nano-spectrum-phy.cc文件
		router->GetPhy()->TraceConnectWithoutContext("outPHYCOLL", MakeBoundCallback(&PrintPHYCOLLEvents, streamPHYCOLL));
		router->TraceConnectWithoutContext("outPHYTX1", MakeBoundCallback(&PrintENERGYEvents, streamENERGY));					//outEnergy底层代码在simple-nano-device.cc文件
	}

	for (uint32_t i = 0; i < d_nodes.GetN(); i++) {
		Ptr<MobilityModel> m = n_nodes.Get(i)->GetObject<MobilityModel> ();
		nano.AddMobility(d_nodes.Get(i)->GetObject<NanoNodeDevice> ()->GetPhy(), m);
		Ptr<NanoNodeDevice> dev = d_nodes.Get(i)->GetObject<NanoNodeDevice> ();

		Ptr<NanoRoutingEntity> routing;
		if (l3Type == 1) {
			Ptr<FloodingNanoRoutingEntity> routing1 = CreateObject<FloodingNanoRoutingEntity> ();
			routing = routing1;
		} else if (l3Type == 2) {
			Ptr<RandomNanoRoutingEntity> routing2 = CreateObject<RandomNanoRoutingEntity> ();
			routing = routing2;
		} else {
			Ptr<OpportunisticNanoRoutingEntity> routing3 = CreateObject<OpportunisticNanoRoutingEntity> ();
			routing = routing3;
		}
		routing->SetDevice(dev);
		dev->SetL3(routing);

		Ptr<NanoMacEntity> mac;
		if (macType == 1) {
			Ptr<TransparentNanoMacEntity> mac1 = CreateObject<TransparentNanoMacEntity> ();
			mac = mac1;
		} else if (macType == 2) {
			Ptr<BackoffBasedNanoMacEntity> mac2 = CreateObject<BackoffBasedNanoMacEntity> ();
		    mac = mac2;
		} else {
			Ptr<FSANanoMacEntity> mac3 = CreateObject<FSANanoMacEntity> ();
			mac = mac3;
		}
		mac->SetDevice(dev);
		dev->SetMac(mac);

		dev->GetPhy()->SetTransmissionRange(txRangeNanoNodes);
		dev->GetPhy()->SetTxPower(powerTransmission);
		dev->GetPhy()->GetObject<TsOokBasedNanoSpectrumPhy> ()->SetPulseDuration(FemtoSeconds(pulseDuration));				//设置物理层协议为TsOokBasedNanoSpectrumPhy
		dev->GetPhy()->GetObject<TsOokBasedNanoSpectrumPhy> ()->SetPulseInterval(FemtoSeconds(pulseInterval));

		dev->GetPhy()->TraceConnectWithoutContext("outPHYTX", MakeBoundCallback(&PrintPHYTXEvents, streamPHYTX));			//outPHYTX与outPHYCOLL底层代码在ts-ook-based-nano-spectrum-phy.cc文件
		dev->GetPhy()->TraceConnectWithoutContext("outPHYCOLL", MakeBoundCallback(&PrintPHYCOLLEvents, streamPHYCOLL));
		dev->TraceConnectWithoutContext("outEnergy", MakeBoundCallback(&PrintENERGYEvents, streamENERGY));					//outEnergy底层代码在simple-nano-device.cc文件

		//物理层之后是节点本身
		//根据距离发生的能量消耗的改变。
		//double energy2distance = txRangeNanoNodes * txRangeNanoNodes / 0.02 / 0.02;
		double energy = 5.0;					//存储能量5J
		double maxenergy = 5.0;					//最大存储能量5J
		double harEnergyInterval = 0.1;			//能量捕获时隙0.1s
		int harEnergySpeed = 760;				//能量捕获速度760pJ/s,除以100,除以2是平均速度。
		//int reduceEnergy = 0;	//for random harvesting
		double EnergySendPerByte = 8.0 / 2.0 * 100.0 * 4.0 / 1000000.0;					//发送能耗/字节1.6*10^-3
		double EnergyReceivePerByte = 8.0 / 2.0 * 100.0 * 4.0 / 1000000.0 / 10.0;		//接收能耗/字节1.6*10^-4
		double PacketSize = 100.0;				//数据包大小100
		double testSize = 50;					//探测数据包大小50,这里的探测数据包是指纳米节点确定候选节点时发送的探测数据包，在mac协议中的forward函数中
		//double U=10;double pram1=0.5;double pram2=0.5;
		//double NACKSize = 9 * energy2distance;
		//uint32_t buffersize = 1;

		dev->SetEnergySendPerByte(EnergySendPerByte);
		dev->SetEnergyReceivePerByte(EnergyReceivePerByte);
		dev->SetPacketSize(PacketSize);
		dev->SetTestSize(testSize);

		dev->SetEnergyCapacity(energy);
		dev->SetMaxEnergy(maxenergy);
		dev->SetHarvestEnergyInterTime(harEnergyInterval);
		dev->SetHarEnergySpeed(harEnergySpeed);
		//dev->SetBufferSize(buffersize);
	}

	//application
	double packetInterval = 0.1;			//纳米节点数据包产生时隙0.1s
	double testPacketInterval = 0.01;		//纳米网关探测数据包产生时隙0.01s

	for (int i = 0; i < nbNanoNodes; i++) {
		Ptr<MessageProcessUnit> mpu = CreateObject<MessageProcessUnit> ();
		mpu->SetDevice(d_nodes.Get(i)->GetObject<SimpleNanoDevice> ());
		d_nodes.Get(i)->GetObject<SimpleNanoDevice> ()->SetMessageProcessUnit(mpu);		//只有纳米节点和纳米网关才设置了信息处理单元并在节点内部函数设置节点传输数据包的大小
		mpu->SetInterarrivalTime(packetInterval);				//设置数据包产生时隙为0.1s
		double startTime = random->GetValue(0.2, 0.3);
		Simulator::Schedule(Seconds(startTime), &MessageProcessUnit::CreteMessage, mpu);		//每个纳米节点调用CreteMessage函数的间隔,间隔为0.2-0.3中一个随机时间
		if(l3Type == 3) {
			Simulator::Schedule(Seconds(0.15), &MessageProcessUnit::computeaverindex, mpu);			//每个纳米节点调用computeaverindex函数的间隔
		}

		// let the node start harvest energy
		Ptr<SimpleNanoDevice> dev = d_nodes.Get(i)->GetObject<SimpleNanoDevice>();
		//可以执行，从一个随机时间开始收集能量
		double harstartTime = random->GetValue(0.3, 0.4);
		Simulator::Schedule(Seconds(harstartTime), &SimpleNanoDevice::HarvestEnergy, dev);			//每个纳米节点调用HarvestEnergy函数的间隔

		mpu->TraceConnectWithoutContext("outTX", MakeBoundCallback(&PrintTXEvents, streamTX));			//outTX与outRX底层代码在message-process-unit.cc文件
		mpu->TraceConnectWithoutContext("outRX", MakeBoundCallback(&PrintRXEvents, streamRX));
	}

//	for (int i = 0; i < nbNanoRouters; i++) {
//		Ptr<MessageProcessUnit> mpu = CreateObject<MessageProcessUnit> ();
//		mpu->SetDevice(d_routers.Get(i)->GetObject<SimpleNanoDevice> ());
//		d_routers.Get(i)->GetObject<SimpleNanoDevice> ()->SetMessageProcessUnit(mpu);
//		mpu->SetInterarrivalTime(packetInterval);
//		mpu->TraceConnectWithoutContext("outTX", MakeBoundCallback(&PrintTXEvents, streamTX));
//		mpu->TraceConnectWithoutContext("outRX", MakeBoundCallback(&PrintRXEvents, streamRX));
//	}

	for (int i = 0; i < nbNanoGateways; i++) {
		Ptr<MessageProcessUnit> mpu = CreateObject<MessageProcessUnit> ();
		mpu->SetDevice(d_gateways.Get(i)->GetObject<SimpleNanoDevice> ());
		d_gateways.Get(i)->GetObject<SimpleNanoDevice> ()->SetMessageProcessUnit(mpu);
		if(l3Type == 3) {
			mpu->SetInterarrivalTestTime(testPacketInterval);				//设置探测数据包产生时隙为0.01s
			double startTime = random->GetValue(0.0, 0.1);
			Simulator::Schedule(Seconds(startTime), &MessageProcessUnit::CretetestMessage, mpu);		//每个网关节点调用CretetestMessage函数的间隔
		}
		mpu->TraceConnectWithoutContext("outTX", MakeBoundCallback(&PrintTXEvents, streamTX));			//outTX与outRX底层代码在message-process-unit.cc文件
		mpu->TraceConnectWithoutContext("outRX", MakeBoundCallback(&PrintRXEvents, streamRX));
	}

	Simulator::Stop(Seconds(duration));
	Simulator::Schedule(Seconds(0.), &PrintSimulationTime, duration);
	Simulator::Schedule(Seconds(duration - 0.1), &PrintMemoryUsage);

	//当Simulator::Run被调用时，系统会开始遍历预设事件的列表并执行，当这些事件被执行后，就没有将来的事件来执行了，函数Simulator::Run会返回。整个模拟过程就结束了
	Simulator::Run();
	//该方法被执行后，模拟器中所有创建的对象将被销毁。你自己并不需要追踪任何对象，你所需要做的仅仅是调用Simulator::Destroy并且退出
	Simulator::Destroy();
}


static void PrintPosition(std::ostream *os, std::string foo, NetDeviceContainer devs) {
	int num = devs.GetN();
	double xrange = 0.5;
	double yrange = 0.02;
	double txRangeNanoNodes = 0.01;
	double Tem = 0;
	double Dirction = 0;
	for (int i = 0; i < num; i++) {
		Ptr<SimpleNanoDevice> d = devs.Get(i)->GetObject<SimpleNanoDevice> ();
		Ptr<MobilityModel> m = d->GetPhy()->GetMobility();
		Vector pos = m->GetPosition();
		Dirction = (pos.x - xrange / 2) * (pos.x - xrange / 2)
				+ (pos.y - yrange / 2) * (pos.y - yrange / 2)
				+ pos.z * pos.z;
		if (Dirction < (txRangeNanoNodes * txRangeNanoNodes))			//纳米网络热量求解代码，Dirction等于公式中1/(d0^2)
			Tem = Tem + (7.567e-13) / Dirction;							//7.567e-13等于公式中c3，公式中gmn等于1，根据ψ=180度计算而来
		*os << Simulator::Now().GetSeconds() << " " << d->GetNode()->GetId() << " " << pos.x << " " << pos.y << " " << pos.z << " " << Tem << std::endl;
	}
	Simulator::Schedule(Seconds(0.1), &PrintPosition, os, foo, devs);
}


void PrintMemoryUsage(void) {
	system("ps aux | grep build/scratch/health-care | head -1 | awk '{print $1, $4, $10}'");
}

//nodeId代表纳米节点的id，packetId代表数据包的序号。收集的是所有普通纳米节点id与其对应节点产生的数据包id。MessageProcessUnit::CreteMessage
void PrintTXEvents(Ptr<OutputStreamWrapper> stream, int nodeId, int packetId) {
	*stream->GetStream() << nodeId << " " << packetId << std::endl;
}

//seqId代表序列号，sourceId代表数据包的发送节点id，nodeId代表接收数据的纳米节点id，为纳米网关节点id，packetSize代表数据包大小，delay数据接收的时延。MessageProcessUnit::ProcessMessage
void PrintRXEvents(Ptr<OutputStreamWrapper> stream, int seqId, int sourceId, int nodeId, int packetSize, double delay) {
	*stream->GetStream()  << seqId << " " << sourceId << " " << nodeId << " " << packetSize <<" "<< delay << std::endl;
}

//energy代表数据包与探测数据包的发送消耗能量与接收消耗能量，用于计算协议的平均传输能耗。SimpleNanoDevice::ConsumeEnergySend和SimpleNanoDevice::ConsumeEnergyReceive
void PrintENERGYEvents(Ptr<OutputStreamWrapper> stream, int nodeId, double consumeEnergy, double nowEnergy) {
	*stream->GetStream() << nodeId << " " << consumeEnergy << " " << nowEnergy << std::endl;
}

//nodeId代表普通纳米节点的id，packetId代表数据包的序号。收集的是物理层中所有数据包与探测数据包发送的源节点id与其发送的数据包id。TsOokBasedNanoSpectrumPhy::StartTx
void PrintPHYTXEvents(Ptr<OutputStreamWrapper> stream, int nodeId, int packetId) {
	*stream->GetStream() << nodeId << " " << packetId << std::endl;
}

//nodeId代表纳米节点的id，packetId代表数据包的序号。收集的是在物理层中接收数据包节点id与其接收的冲突数据包id。TsOokBasedNanoSpectrumPhy::EndRx
void PrintPHYCOLLEvents(Ptr<OutputStreamWrapper> stream, int nodeId, int packetId) {
	*stream->GetStream() << nodeId << " " << packetId << std::endl;
}

void PrintSimulationTime(double duration) {
	//Simulator::Now().GetSeconds()代表程序现在的执行时间
	double percentage = (100. * Simulator::Now().GetSeconds()) / duration;
	std::cout << "*** " << "Simulator::Now().GetSeconds():" << Simulator::Now().GetSeconds() << " " << percentage << "%" << " *** " << std::endl;

	//double deltaT = duration / 10;
	//int t = Simulator::Now().GetSeconds() / deltaT;
	//double nexttime = deltaT * (t + 1);

	//第一个参数代表下一次执行的间隔时间
	Simulator::Schedule(Seconds(0.5), &PrintSimulationTime, duration);
}
