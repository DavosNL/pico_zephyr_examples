#include <zephyr/kernel.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_if.h>
#include <zephyr/net/net_ip.h>

static struct net_mgmt_event_callback wifi_cb;
static K_SEM_DEFINE(wifi_connected, 0, 1);

static void wifi_event_handler(struct net_mgmt_event_callback *cb,
                                uint64_t mgmt_event, struct net_if *iface)
{
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        printk("Connected to Wi-Fi!\n");

        //Short delay to allow DHCP to complete
        k_sleep(K_SECONDS(1));

        // Read the IP address directly from the interface
        struct net_if_ipv4 *ipv4 = iface->config.ip.ipv4;
        if (ipv4 != NULL) {
            char ip_str[NET_IPV4_ADDR_LEN];
            net_addr_ntop(AF_INET,
                          &ipv4->unicast[0].ipv4.address.in_addr,
                          ip_str, sizeof(ip_str));
            printk("IP address: %s\n", ip_str);
        } else {
            printk("No IP address found\n");
        }

        k_sem_give(&wifi_connected); 

    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        printk("WiFi disconnected.\n");
    }
}

int main(void)
{
    struct net_if *iface = net_if_get_default();
    struct wifi_connect_req_params params = {
        .ssid        = WIFI_SSID,
        .ssid_length = strlen(WIFI_SSID),
        .psk         = WIFI_PSK,
        .psk_length  = strlen(WIFI_PSK),
        .security    = WIFI_SECURITY_TYPE_PSK,
    };

    net_mgmt_init_event_callback(&wifi_cb, wifi_event_handler,
        NET_EVENT_WIFI_CONNECT_RESULT | 
        NET_EVENT_WIFI_DISCONNECT_RESULT);
    net_mgmt_add_event_callback(&wifi_cb);

    printk("Connect to %s...\n", WIFI_SSID);
    int ret = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &params, sizeof(params));
    if (ret) {
        printk("Connection failed: %d\n", ret);
        return ret;
    }

    // Waiting for connection
    k_sem_take(&wifi_connected, K_SECONDS(30));


    return 0;
}