#pragma once
#include <juce_osc/juce_osc.h>

/**
 An extension to JUCE's OSCReceiver class with some useful methods.
 */

class OSCReceiverPlus : public juce::OSCReceiver
{
public:
  OSCReceiverPlus() { connected = false; }

  bool connect(int portNumber)
  {
    port = portNumber;
    if (portNumber == -1) {
      disconnect();
      connected = false;
      return true;
    }

    if (OSCReceiver::connect(port)) {
      connected = true;
      return true;
    } else
      return false;
  }

  bool disconnect()
  {
    if (OSCReceiver::disconnect()) {
      connected = false;
      return true;
    } else
      return false;
  }

  int getPortNumber() const { return port; }

  bool isConnected() const { return connected.get(); }

private:
  int port = -1;
  juce::Atomic<bool> connected;
};

class OSCSenderPlus : public juce::OSCSender
{
public:
  OSCSenderPlus() { connected = false; }

  bool connect(const juce::String& targetHostName, int portNumber)
  {
    hostName = targetHostName;
    port = portNumber;

    if (portNumber == -1 || targetHostName.isEmpty()) {
      disconnect();
      connected = false;
      return true;
    }

    if (juce::OSCSender::connect(targetHostName, port)) {
      connected = true;
      return true;
    } else
      return false;
  }

  bool disconnect()
  {
    if (OSCSender::disconnect()) {
      connected = false;
      return true;
    } else
      return false;
  }

  int getPortNumber() const { return port; }

  juce::String getHostName() const { return hostName; }

  bool isConnected() const { return connected.get(); }

private:
  juce::String hostName;
  int port = -1;
  juce::Atomic<bool> connected;
};

class OSCMessageInterceptor
{
public:
  virtual ~OSCMessageInterceptor() = default;

  /**
   This method is exptected to return true, if the juce::OSCMessage is considered to have been
   consumed, and should not be passed on.
   */
  virtual inline const bool interceptOSCMessage(juce::OSCMessage& message)
  {
    ignoreUnused(message);
    return false; // not consumed
  }

  /**
   This method will be called if the OSC message wasn't consumed by both 'interceptOscMessage(...)'
   and the oscParameterInterface.processOSCmessage(...)' method. The method is expected to return
   true, if the SOCMessage is considered to have been consumed, and should not be passed on.
   */
  virtual inline const bool processNotYetConsumedOSCMessage(const juce::OSCMessage& message)
  {
    ignoreUnused(message);
    return false;
  }

  /**
   Use this method to send additional juce::OSCMessages during the OSCSender's send routine.
   */
  virtual void sendAdditionalOSCMessages(juce::OSCSender&, const juce::OSCAddressPattern&) {}
};
