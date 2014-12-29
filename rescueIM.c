#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "nanomsg/nn.h"
#include "nanomsg/reqrep.h"
#include "log.h"
#include "config.h"

char NANOMSG_REP_URL[32] = {0x00};
log4c_category_t *gLog;

int main (int argc, char *argv [])
{
	int req, rc;

	getConfString ("REP", NANOMSG_REP_URL);

	log4c_init ();
	gLog = log4c_category_get ("log.std");

	LOG_INFO (gLog, "system start !!");
	LOG_INFO (gLog, "server : %s", NANOMSG_REP_URL);

	req = nn_socket (AF_SP, NN_REQ);
	if (req < 0)
		LOG_ERROR (gLog, "nn_socket fail!! %s", strerror (errno));

	rc = nn_connect (req, NANOMSG_REP_URL);
	if (rc < 0)
		LOG_ERROR (gLog, "nn_connect fail!! %s", strerror (errno));

	char pid[32] = {0x00};
	char price[32] = {0x00};
	char qty[32] = {0x00};
	char type[32] = {0x00};

	while (1)
	{
		printf ("\n*********************************\n");
		printf ("Usage : [Q:quote T:tick] [symbol] [price] [volumn]\n");
		printf ("input >>");

		scanf ("%s %s %s %s", type, pid, price, qty);
		LOG_INFO (gLog, "param : %s %s %s %s", type, pid, price, qty);
		
		if (atof (price) == 0)
		{
			printf ("*** [price] must be float!! ***\n");
			continue;
		}
		if (atoi (qty) == 0)
		{
			printf ("*** [volumn] must be integer!! ***\n");
			continue;
		}

		char reqMsg[128] = {0x00};
		char repMsg[512] = {0x00};
		snprintf (reqMsg, sizeof (reqMsg), "#%s#M%s#%s#%s#", pid, type, price, qty);
		LOG_INFO (gLog, "request : %s", reqMsg);
		rc = nn_send (req, reqMsg, strlen (reqMsg), 0);
		if (rc < 0)
		{
			LOG_ERROR (gLog, "nn_send fail!! %s", strerror (errno));
			continue;
		}
		rc = nn_recv (req, repMsg, sizeof (repMsg), 0);
		if (rc < 0)
		{
			LOG_ERROR (gLog, "nn_recv fail!! %s", strerror (errno));
			continue;
		}
		LOG_INFO (gLog, "reply = %s", repMsg);
	}

	return 0;
}

