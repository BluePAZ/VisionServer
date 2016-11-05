#include "capi_server.h"

int main(int argc, char * argv[]){
	init();
	pthread_create(&server_connect_thread, NULL, server_accept_thread, NULL);
	pthread_join(server_connect_thread, NULL);
	return 0;
}
void init(){
	shared_data = DataSharer.get_instance();
	if(shared_data != NULL){
		fprintf("Shared Data Initialized...\n");
	}
	taskFactory = WorkTaskFactory::get();
	IVortexNIFSAP * AcceleratorInterface = new nifsap_capi_v2_00_b(false,0,"",7,0,0);
	if (!AcceleratorInterface->Initialize(0))
	{
		fprintf(stderr, "Failed to init accelerator\n");
		delete(AcceleratorInterface);
		return;
	}
	AcceleratorInterface->AddMemoryInterface(0,0,0);
	dispatcher = new WorkManagerDispatcher();
#if(WITH_AIM)
	AIMWorkManager *aimMng = new AIMWorkManager();
	aimMng->enqueueWorker(new AIMWorkConsumer(AcceleratorInterface));
	dispatcher->addManager(aimMng);

	AIMPostProcessWorkManager *aimPostMng = new AIMPostProcessWorkManager();
	aimPostMng->enqueueWorker(new AIMPostProcessGPUWorkConsumer());
	//aimPostMng->enqueueWorker(new AIMPostProcessWorkConsumer());
	//aimPostMng->enqueueWorker(new AIMPostProcessWorkConsumer());
	//aimPostMng->enqueueWorker(new AIMPostProcessWorkConsumer());
	//aimPostMng->enqueueWorker(new AIMPostProcessWorkConsumer());
	//aimPostMng->enqueueWorker(new AIMPostProcessWorkConsumer());
	dispatcher->addManager(aimPostMng);
	taskFactory->registerTask(AIMWorkTask::returnTaskName(), &AIMWorkTask::Create);
#endif
#if(WITH_SURF)
	OpenCVSurfBFFMatcherWorkManager *surfBffManager = new OpenCVSurfBFFMatcherWorkManager();
	surfBffManager->enqueueWorker(new OpenCVSurfBFFMatcherWorkConsumer());

	OpenCVSurfIpDescriptorWorkManager *surfIpDescManager = new OpenCVSurfIpDescriptorWorkManager();
	surfIpDescManager->enqueueWorker(new OpenCVSurfIpDescriptorWorkConsumer());

	OpenCVSurfIpWorkManager *surfIpManager = new OpenCVSurfIpWorkManager();
	surfIpManager->enqueueWorker(new OpenCVSurfIpWorkConsumer());

	OpenCVSurfDescriptorWorkManager *surfDescManager = new OpenCVSurfDescriptorWorkManager();
	surfDescManager->enqueueWorker(new OpenCVSurfDescriptorWorkConsumer());

	dispatcher->addManager(surfIpDescManager);
	dispatcher->addManager(surfBffManager);
	dispatcher->addManager(surfDescManager);
	dispatcher->addManager(surfIpManager);

	taskFactory->registerTask(OpenCVSurfIpWorkTask::returnTaskName(), &OpenCVSurfIpWorkTask::Create);
	taskFactory->registerTask(OpenCVSurfFullWorkTask::returnTaskName(), &OpenCVSurfFullWorkTask::Create);
	taskFactory->registerTask(OpenCVSurfFullDemoWorkTask::returnTaskName(), &OpenCVSurfFullDemoWorkTask::Create);
#endif
#if(WITH_GPU_SURF)
	///////////////////////////////////////////////
	//add the gpu version to the cpu queues as well
	///////////////////////////////////////////////
	surfIpDescManager->enqueueWorker(new OpenCVGpuSurfIpDescriptorWorkConsumer());
	surfIpManager->enqueueWorker(new OpenCVGpuSurfIpDescriptorWorkConsumer());
	surfDescManager->enqueueWorker(new OpenCVGpuSurfDescriptorWorkConsumer());
	/////////////////////////////////////////////////
	//Dedicated Queues///////////////////////////////
	/////////////////////////////////////////////////
	OpenCVGpuSurfIpDescriptorWorkManager *surfGPUIpDescManager = new OpenCVGpuSurfIpDescriptorWorkManager();
	surfGPUIpDescManager->enqueueWorker(new OpenCVGpuSurfIpDescriptorWorkConsumer());
	OpenCVGpuSurfDescriptorWorkManager *surfGPUDescManager = new OpenCVGpuSurfDescriptorWorkManager();
	surfGPUDescManager->enqueueWorker(new OpenCVGpuSurfDescriptorWorkConsumer());
	OpenCVGpuBFFMatcherWorkManager * gpuBFFMatcher = new OpenCVGpuBFFMatcherWorkManager();
	gpuBFFMatcher->enqueueWorker(new OpenCVGpuBFFMatcherWorkConsumer());
	///////////////////////////////////////////////////
	dispatcher->addManager(surfGPUIpDescManager);
	dispatcher->addManager(surfGPUDescManager);
	dispatcher->addManager(gpuBFFMatcher);
#endif
#if(WITH_HOG)
	HOGWorkManager *hogManager = new HOGWorkManager();
	hogManager->enqueueWorker(new HOGWorkConsumer(AcceleratorInterface));

	SVMWorkManager *svmManager = new SVMWorkManager();
	svmManager->enqueueWorker(new SVMWorkConsumer(AcceleratorInterface));

	dispatcher->addManager(hogManager);
	dispatcher->addManager(svmManager);

	taskFactory->registerTask(PersonDetectorPipelineWorkTask::returnTaskName(), &PersonDetectorPipelineWorkTask::Create);
#endif
#if(WITH_CMT)
	taskFactory->registerTask(CMTWorkTask::returnTaskName(), &CMTWorkTask::Create);
#endif
}
void * server_accept_thread(void * server_start_data_args){
	fprintf(stderr," Server Thread Started!\n");
	srand(time(0));
	int status;
	struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
	struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.
	// The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
	// to by hints must contain either 0 or a null pointer, as appropriate." When a struct
	// is created in C++, it will be given a block of memory. This memory is not necessary
	// empty. Therefor we use the memset function to make sure all fields are NULL.
	memset(&host_info, 0, sizeof host_info);
	std::cout << "Setting up the structs..."  << std::endl;
	host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
	host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.
	// Now fill up the linked list of host_info structs with google's address information.
	status = getaddrinfo("0.0.0.0", "2275", &host_info, &host_info_list);
	// getaddrinfo returns 0 on succes, or some other value when an error occured.
	// (translated into human readable text by the gai_gai_strerror function).
	if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;
	std::cout << "Creating a socket..."  << std::endl;
	int socketfd ; // The socket descripter
	socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);
	if (socketfd == -1)  std::cout << "socket error " ;
	client_count = 0;
	std::cout << "Binding socket..."  << std::endl;
	// we make use of the setsockopt() function to make sure the port is not in use.
	// by a previous execution of our code. (see man page for more information)
	int yes = 1;
	status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	if (status == -1)  std::cout << "bind error" << std::endl ;

	/* listen to accept connections connections */
	std::cout << "Listening for connections..."  << std::endl;
	status =  listen(socketfd, 1);
	if (status == -1)  {
		std::cout << "listen error" << std::endl ;
		printf("read()! %s\n", strerror(errno));
	}
	while(1){
		sockaddr_in clientAddr;
		socklen_t sin_size=sizeof(struct sockaddr_in);
		int new_fd = accept(socketfd, (struct sockaddr*)&clientAddr, &sin_size);
		client_count++;
		socket_data data_packet;
		if(client_count > (MAX_SOCKET_CONNECTIONS)){    //Drop connections and send message with disconnect reason
			fprintf(stderr, "To many connections\n %i", client_count--);
			data_packet.message_type = CONNECTION_DISCONNECT;
			data_packet.message_length = 0;
			data_packet.stream_id = -1;
			data_packet.message_id = rand() % RAND_MAX;
			data_packet.frame_id = 0;
			send_data_packet(new_fd, data_packet);
			close(new_fd);
		}else{
			//fully open connections returning stream_id
			fprintf(stderr, "Connected new client! %i\n", client_count);
			image_server_thread_data_struct * td = (image_server_thread_data_struct *)malloc(sizeof(image_server_thread_data_struct));
			td->stream_id = ++client_id;
			td->socket_fd = new_fd;
			pthread_create(&threads[client_count], NULL, image_server_thread, (void *)td);
		}
	}
	pthread_exit(0);
}
void * image_server_thread(void * image_server_thread_data_args){
	fprintf(stderr, "thread started ... client connected\n");
   	image_server_thread_data_struct * image_server_thread_data;
	image_server_thread_data = (image_server_thread_data_struct *) image_server_thread_data_args;

	uint32_t stream_id = image_server_thread_data->stream_id;
	int socketfd = image_server_thread_data->socket_fd;
	free(image_server_thread_data); //done with args -- clean up memory
	/*
	 * CPP Connection Accept Code
	 */
	CAPINetworkUtility *tmpCntrl = new CAPINetworkUtility(socketfd);
	int err = 0;
	SocketData * tmpPkt;
	tmpPkt = tmpCntrl->receiveDataPacket(&err);
	fprintf(stderr, "packet received\n");
	if(err > 0){
		fprintf(stderr, "error from packet receive\n");
		close(socketfd);
		return NULL;
	}
	if(tmpPkt != NULL){
		tmpPkt->printPacket();
		switch(tmpPkt->message_type){
			case REGISTER_TASK:{
				/* add logic to parse task out */
				std::vector<std::string> configs = parseConfig(tmpPkt->data, tmpPkt->message_length);
				fprintf(stderr, "configs parsed count: %i\n", configs.size());
				WorkTask * serverWorkTask = taskFactory->CreateTask(configs[0]);
				fprintf(stderr, "created task\n");

				if(serverWorkTask != NULL){
					fprintf(stderr, "created task not null\n");
					serverWorkTask->dispatcher = dispatcher;
					fprintf(stderr, "created task dispatcher\n");
					serverWorkTask->shared_data = shared_data;
					serverWorkTask->registerSocket(socketfd, stream_id);
					serverWorkTask->startTask();
				}else{
					fprintf(stderr, "task null\n");
				}
				break;
			}
		}
	}else{
		fprintf(stderr, "packet is null oops\n");
	}
	fprintf(stderr, "exiting launch thread\n");
	delete tmpPkt;
}
std::vector<std::string> parseConfig(uint8_t * data, uint32_t length){
	std::string * rawString = new std::string((char *)data, length);
	std::vector<std::string> config = split(*rawString, ':');
	delete rawString;
	return config;
}
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
