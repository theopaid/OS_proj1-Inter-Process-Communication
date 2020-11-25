#ifndef _CHANNEL_H
#define _CHANNEL_H

ConnectionDetails *setUpLinkingWithENC1();

ConnectionDetails *setUpLinkingWithENC2();

void interactWithENC1andENC2(ConnectionDetails *connectionENC1, ConnectionDetails *connectionENC2);

#endif