#pragma once
#include "wifiState.hpp"
#include "wifiContext.hpp"

class wifiStateIdle; // Forward declaration

// State: Disconnected
class wifiStateDisconnected : public wifiState {
private:
    wifiStateIdle* idle;

public:
    wifiStateDisconnected(wifiStateIdle* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
};

// State: Connected
class wifiStateConnected : public wifiState {
private:
    wifiStateDisconnected* disconnected;

public:
    wifiStateConnected(wifiStateDisconnected* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
};

// State: Connecting
class wifiStateConnecting : public wifiState {
private:
    wifiStateConnected* connected;

public:
    wifiStateConnecting(wifiStateConnected* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
};

// State: Idle
class wifiStateIdle : public wifiState {
private:
    wifiStateConnecting* connecting;

public:
    wifiStateIdle(wifiStateConnecting* next);
    void enter(wifiContext& ctx) override;
    void handle(wifiContext& ctx) override;
    const char* name() const override;
};


// // State: Disconnected
// class wifiStateDisconnected : public wifiState {
// private:
//     wifiStateIdle* idle;
// public:
//     void enter(wifiContext& ctx) override {
//         MYLOG("❌ Disconnected. Awaiting reconnection...");
//     }

//     void handle(wifiContext& ctx) override {
//         MYLOG("🔄 Trying to recover or restart connection");
//         bool recoveryComplete = true; // Replace with real logic
//         if (recoveryComplete) {
//             ctx.setState(static_cast<wifiState*>(idle));
//         }
//     }

//     const char* name() const override { return "Disconnected"; }
// };

// // State: Connected
// class wifiStateConnected : public wifiState {
// private:
// wifiStateDisconnected* disconnected;

// public:
//     wifiStateConnected(wifiStateDisconnected* next) : disconnected(next) {}

//     void enter(wifiContext& ctx) override {
//         MYLOG("📶 Connected! Holding...");
//     }

//     void handle(wifiContext& ctx) override {
//         MYLOG("📡 Maintaining connection");
//         bool lostConnection = true; // Replace with real check
//         if (lostConnection) {
//             ctx.setState(disconnected);
//         }
//     }

//     const char* name() const override { return "Connected"; }
// };

// // State: Connecting
// class wifiStateConnecting : public wifiState {
// private:
//     wifiStateConnected* connected;

// public:
//     wifiStateConnecting(wifiStateConnected* next) : connected(next) {}

//     void enter(wifiContext& ctx) override {
//         MYLOG("🔗 Entered Connecting state");
//     }

//     void handle(wifiContext& ctx) override {
//         MYLOG("✅ Connected. Switching to Connected state...");
//         ctx.setState(connected);
//     }

//     const char* name() const override { return "Connecting"; }
// };

// // State: Idle
// class wifiStateIdle : public wifiState {
// private:
//     wifiStateConnecting* connecting;

// public:
//     wifiStateIdle(wifiStateConnecting* next) : connecting(next) {}

//     void enter(wifiContext& ctx) override {
//         MYLOG("🟡 Entered Idle state");
//     }

//     void handle(wifiContext& ctx) override {
//         MYLOG("➡️ Transitioning to Connecting...");
//         ctx.setState(connecting);
//     }

//     const char* name() const override { return "Idle"; }
// };

// #pragma once
// #include "wifiState.hpp"
// #include "wifiContext.hpp"
// #include "myLogger.h"

// // 👇 Add forward declarations
// class scanningStateImp;
// class connectingStateImp;
// class connectedStateImp;
// class errorStateImp;
// class idleStateImp;

// // 👇 Now declare extern variables
// // extern scanningStateImp scanning;
// // extern connectingStateImp connecting;
// // extern connectedStateImp connected;
// // extern errorStateImp error;
// // extern idleStateImp idle;

// class connectedStateImp : public wifiState {
// public:
//     void enter(wifiContext& ctx) override {
//         MYLOG("✅ Wi-Fi Connected!");
//     }

//     void handle(wifiContext& ctx) override {
//         // Maintain or monitor connection
//     }

//     const char* name() const override { return "Connected"; }
// };

// class connectingStateImp : public wifiState {
// public:
//     void enter(wifiContext& ctx) override {
//         MYLOG("🔗 Entering connectingState");
//         // net_mgmt(NET_REQUEST_WIFI_CONNECT, ...);
//     }

//     void handle(wifiContext& ctx) override {
//         bool isConnected = true; // Replace with actual connection status
//         // ctx.setState(isConnected ? static_cast<wifiState*>(&connected) : static_cast<wifiState*>(&error));
//     }

//     const char* name() const override { return "Connecting"; }
// };

// class scanningStateImp : public wifiState {
// public:
//     void enter(wifiContext& ctx) override {
//         MYLOG("🔍 Entering scanningState");
//         // net_mgmt(NET_REQUEST_WIFI_SCAN, ...);
//     }

//     void handle(wifiContext& ctx) override {
//         MYLOG("📶 Found networks, connecting...");
//         ctx.setState(&connecting);
//     }

//     const char* name() const override { return "Scanning"; }
// };

// class idleStateImp : public wifiState {
// public:
//     void enter(wifiContext& ctx) override {
//         MYLOG("🟡 Entering idleState");
//     }

//     void handle(wifiContext& ctx) override {
//         MYLOG("➡️  Starting scan...");
//         ctx.setState(&scanning);
//     }

//     const char* name() const override { return "Idle"; }
// };

// class errorStateImp : public wifiState {
// public:
//     void enter(wifiContext& ctx) override {
//         MYLOG("❌ Connection failed. Going back to idle.");
//     }

//     void handle(wifiContext& ctx) override {
//         ctx.setState(&idle);
//     }

//     const char* name() const override { return "Error"; }
// };