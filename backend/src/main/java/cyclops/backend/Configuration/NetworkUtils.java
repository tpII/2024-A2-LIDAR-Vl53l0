package cyclops.backend.Configuration;

import java.io.IOException;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;

public class NetworkUtils {
    public static boolean isHostAvailable(String ip, int timeout) {
        try {
            return InetAddress.getByName(ip).isReachable(timeout);
        } catch (IOException e) {
            return false;
        }
    }

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
