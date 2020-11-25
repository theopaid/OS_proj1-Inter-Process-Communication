#ifndef _ENCODER_H
#define _ENCODER_H

ConnectionDetails *setUpLinkingWithP1();

ConnectionDetails *setUpLinkingWithChannel();

ConnectionDetails *setUpLinkingENC2toP2();

ConnectionDetails *setUpLinkingENC2toChannel();

void interactWithP1andChannel(ConnectionDetails *connectionP1, ConnectionDetails *connectionChannel);

void interactWithP2andChannel(ConnectionDetails *connectionP2, ConnectionDetails *connectionChannel);

#endif