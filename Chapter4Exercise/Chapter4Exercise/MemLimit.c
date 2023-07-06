/*
Write a tool called MemLimit that accepts a process ID and a number representing the
maximum committed memory for the process and set that limit using a job.

I will just call the application Chapter4Exercise.exe, but the concept is still the same
Usage: Chapter4Exercise.exe <PID> <Memory in MB>
Use sysinternals suite testlimit64.exe -d 2024 -c 1 to test this program out
make sure to run the Chapter4Exercise to the command prompt PID prior to running the program
explanation on why this is is below:

https://learn.microsoft.com/en-us/windows/win32/api/jobapi2/nf-jobapi2-assignprocesstojobobject#remarks
Memory operations performed by a process associated with a job that has a memory limit are subject to the memory limit. 
Memory operations performed by the process before it was associated with the job are not examined by AssignProcessToJobObject.
You can only limit the memory of a process or job prior to running this program
If you wait till the memory is already allocated, it will not work



*/

#include<windows.h>
#include<stdio.h>


int main(int argc, const char* argv[]) {

	if (argc < 3) {
		printf("Usage: Chapter4Exercise <pid> <memory limit in MB>\n ");
		return 1;
	}

	DWORD pid = atoi(argv[1]);
	HANDLE hJob = CreateJobObject(NULL, L"MemLimit");
	HANDLE hProc = OpenProcess(PROCESS_SET_QUOTA | PROCESS_TERMINATE,FALSE, pid);
	
	


	if (!hJob) {
		printf("Failed to create job, GetLastError() = %d\n",GetLastError());
		return 1;
	}
	

	if (!hProc) {
		printf("Failed to OpenProcess, GetLastError() = %d\n", GetLastError());
		return 1;
	}
	//Assigning Process to Job so we can enforce memory limits on the job that will affect the process
	if (!AssignProcessToJobObject(hJob,hProc)) {
		printf("Failed to AssignProcessToJobObject, GetLastError() = %d\n", GetLastError());
		return 1;
	}

	//Creating the struct to set the limit on the job object
	JOBOBJECT_EXTENDED_LIMIT_INFORMATION info;
	info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_JOB_MEMORY;
	info.JobMemoryLimit = atoi(argv[2]) * 1048576;//multiply by 1,048,576 to get the amount in bytes
					

	if (!SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &info, sizeof(info))) {
		printf("Failed to SetInformationJobObject, GetLastError() = %d\n", GetLastError());
		return 1;
	}

	
	else {
		printf("The process %d has it's memory restricted to %d bytes!!!\n,press ENTER to close program\n",pid,atoi(argv[2]) * 1048576);
		getchar();
		getchar();
	}

	TerminateJobObject(hJob, 0);
	CloseHandle(hJob);
	CloseHandle(hProc);


	return EXIT_SUCCESS;
}