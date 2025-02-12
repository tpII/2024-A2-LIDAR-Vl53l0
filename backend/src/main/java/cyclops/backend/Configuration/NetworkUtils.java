/**
 * Utility class for network-related operations.
 */
package cyclops.backend.Configuration;

import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

public class NetworkUtils {

    /**
     * Checks if a host with the given IP address is reachable within the specified timeout.
     *
     * @param ip      The IP address of the host to check.
     * @param timeout The timeout in milliseconds.
     * @return true if the host is reachable, false otherwise.
     */
    public static boolean isHostAvailable(String ip, int timeout) {
        try {
            return InetAddress.getByName(ip).isReachable(timeout);
        } catch (IOException e) {
            return false;
        }
    }

     /**
     * Checks if the current machine's IP address matches the expected IP address.
     *
     * @param expectedIp The expected IP address to verify.
     * @return true if the expected IP matches any of the machine's network interfaces, false otherwise.
     */
    public static boolean isMyIp(String expectedIp) {
        try {
            Enumeration<NetworkInterface> networkInterfaces = NetworkInterface.getNetworkInterfaces();
            while (networkInterfaces.hasMoreElements()) {
                NetworkInterface networkInterface = networkInterfaces.nextElement();

                // Evita interfaces no operativas (como adaptadores virtuales).
                if (!networkInterface.isUp() || networkInterface.isLoopback()) {
                    continue;
                }

                Enumeration<InetAddress> addresses = networkInterface.getInetAddresses();
                while (addresses.hasMoreElements()) {
                    InetAddress inetAddress = addresses.nextElement();

                    // Compara con la IP esperada.
                    if (inetAddress.getHostAddress().equals(expectedIp)) {
                        return true;
                    }
                }
            }
        } catch (SocketException e) {
            e.printStackTrace();
        }
        return false;
    }
}
