package net.searock.apikeysinndk;

import android.util.Base64;

import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;

public class Encryption {

    public String encryptData(String key, String data) throws Exception {

        String encryptedData;
        try {
            SecretKeySpec skeyspec = new SecretKeySpec(key.getBytes(), "Blowfish");
            Cipher cipher = Cipher.getInstance("Blowfish/ECB/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, skeyspec);
            byte[] encrypted = cipher.doFinal(data.getBytes());
            encryptedData = Base64.encodeToString(encrypted, Base64.NO_WRAP);

        } catch (Exception e) {
            e.printStackTrace();
            throw new Exception(e);
        }
        return encryptedData;


    }

    public String decryptData(String key, String data) throws Exception {

        String decryptedData;

        try {
            SecretKeySpec skeyspec = new SecretKeySpec(key.getBytes(), "Blowfish");
            Cipher cipher = Cipher.getInstance("Blowfish/ECB/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, skeyspec);
            byte[] encryptedBytes = Base64.decode(data, Base64.NO_WRAP);
            byte[] decrypted = cipher.doFinal(encryptedBytes);
            decryptedData = new String(decrypted);

        } catch (Exception e) {
            e.printStackTrace();
            throw new Exception(e);
        }
        return decryptedData;
    }
}
