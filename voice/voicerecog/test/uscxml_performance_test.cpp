#include <sys/time.h>

#include "uscxml/config.h"
#include "uscxml/Interpreter.h"
#include "uscxml/DOMUtils.h"

// #ifndef BUILD_MINIMAL
// #	include "uscxml/debug/DebuggerServlet.h"
// #endif
#include <glog/logging.h>

#include "uscxml/Factory.h"
#include "uscxml/server/HTTPServer.h"

#ifdef HAS_SIGNAL_H
#include <signal.h>
#endif

#ifdef HAS_EXECINFO_H
#include <execinfo.h>
#endif

#ifdef HAS_DLFCN_H
#include <dlfcn.h>
#endif

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
using namespace uscxml;

class VerboseMonitor : public uscxml::InterpreterMonitor {
};



static std::string documentURL = "./test.xml";

void printLocalTime()
{
	char buff[100];
	time_t now = time (0);
	strftime (buff, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
	printf ("%s\n", buff);
}

void print_msg(const char* msg)
{
//    static char buff[100];
//    time_t now = time (0);
//    strftime (buff, 100, "%Y-%m-%d %H:%M:%S.000", localtime(&now));
//    printf ("%s %s\n", buff, msg);
    struct timeval t_cur;
    gettimeofday(&t_cur, NULL);
    printf("%10ld us\t%s\n", t_cur.tv_usec, msg);
}

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}


void printMemoryUse(int *i)
{
	double vm, rss;
   	process_mem_usage(vm, rss);
    printf("in Interpreter %p thread, VM:%f     RSS:%f  \n", i, vm, rss);
}

void fun(void* p)
{
    int* pi = (int*)p;
	while (true)
	{

        printMemoryUse(pi);
		// VerboseMonitor* vm = new VerboseMonitor();
		Interpreter interpreterParent;
        printf("\n\r loading interpreter %p \n\r", pi);
		interpreterParent = Interpreter::fromURL(documentURL);
        printf("\n\r load interpreter %p end \n\r", pi);
		// interpreterParent.addMonitor(vm);
		interpreterParent.start();
        printf("start interpreter %p, sleep 1s \n", pi);
		usleep(3000 * 1000);
		interpreterParent.stop();
		printLocalTime();

		while (interpreterParent.isRunning())
		{
            printf("interpreter %p is Running\n", pi);
			uscxml::Event evt;
			evt.setName("send.over");
			evt.setEventType(Event::EXTERNAL);
			interpreterParent.receive(evt);
			printf("Interpreter receive evet send.over\n");
			// usleep(500 * 1000);
			// interpreterParent.stop();
			usleep(1000 * 1000);
		}
        printf("Interpreter %p is end\n", pi);
		printLocalTime();
	}
}

double printMemoryUse_cxh(const char* msg)
{
    double vm, rss;
    process_mem_usage(vm, rss);
    printf("VM:%f\tRSS:%f\t%s\n", vm, rss, msg);

    return vm;
}

Interpreter fun_cxh()
{
    printLocalTime();
    double vm1 = printMemoryUse_cxh("===== Begine ====");

    Interpreter interpreter = Interpreter::fromURL(documentURL);
    sleep(1);
    printMemoryUse_cxh("build");

    interpreter.start();
    sleep(1);
    printMemoryUse_cxh("start");

    interpreter.stop();
    sleep(1);
    printMemoryUse_cxh("stop");

    while (interpreter.isRunning())
    {
        printf("is running-----\n");
        uscxml::Event evt;
        evt.setName("send.over");
        evt.setEventType(Event::EXTERNAL);
        interpreter.receive(evt);
        sleep(1);
    }

    double vm2 = printMemoryUse_cxh("End");
    printf("---*** vm increase: %f \n", vm2-vm1);

    return interpreter;
}

class StateTransPerfMonitor : public uscxml::InterpreterMonitor {
    virtual void beforeEnteringState(Interpreter interpreter, const Arabica::DOM::Element<std::string>& state, bool moreComing)
    {
        print_msg(state.getAttribute("id").c_str());
    }
};

void fun_test_performance(const char* filepath)
{
    print_msg("start test...");
    StateTransPerfMonitor* vm = new StateTransPerfMonitor();

    print_msg(filepath);
//    Interpreter interpreter = Interpreter::fromURL(std::string("./test1.xml"));
    Interpreter interpreter = Interpreter::fromURL(filepath);
    interpreter.addMonitor(vm);
    interpreter.start();
    sleep(2);

    interpreter.stop();
    sleep(1);
    while (interpreter.isRunning())
    {
        printf("is running-----\n");
        uscxml::Event evt;
        evt.setName("send.over");
        evt.setEventType(Event::EXTERNAL);
        interpreter.receive(evt);
        sleep(1);
    }
    print_msg("end test....");
}

int main(int argc, char* argv[])
{
//    fun_cxh();
//    printf("---*** vm increase: %f \n", vm2-vm1);

//    Interpreter interpreter;
//    for (int xx = 0; xx < 5; ++xx) {
//        std::cout << "======= " << xx << " ==========" << std::endl;
//        interpreter = fun_cxh();
//    }
//    Interpreter::fromURL(documentURL);
    printf("SCXML: %s\n", argv[1]);
    fun_test_performance(argv[1]);
}
