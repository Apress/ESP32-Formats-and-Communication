/*******************************************************************************
 * Sketch name: Send email with message and images
 * Description: Image stored in PROGMEM or SPIFFS
 * Created on:  February 2023
 * Author:      Neil Cameron
 * Book:        ESP32 Communication
 * Chapter :    7 - email and QR codes
 ******************************************************************************/

#include <ESP_Mail_Client.h>        // include library
#include <ssid_password.h>        // file with logon details
#include "logo.h"           // include image data tab
//#include "cap.h"
SMTPSession smtp;         // associate smtp with SMTP connection
ESP_Mail_Session session;       //     session with email access,
SMTP_Message message;         //     message with email content
SMTP_Attachment att;          //     and att with attachments
//char emailSender[] = "xxxx";  // change xxxx to sender email
//char emailPassword[] = "xxxx";    //     and xxxx to sender password
char emailName[] = "from ESP32";        // sender name (text)
char emailSubject[] = "email sent by ESP32";    // email subject
char emailRecipient[] = "xxxx"; // change xxxx to recipient
char emailRcptName[] = "xxxx";       //     and to recipient name (text)
char emailHost[] = "smtp.gmail.com";
uint16_t emailPort = 465;
String baseStr = "sample text for message ";
String str, baseHTML, header, newStr, newHTML;
int switchPin = 0, LEDpin = 2;     // switch and LED pins
int counter = 0;

void setup() 
{
  Serial.begin(115200);
  delay(500);
  pinMode(switchPin, INPUT_PULLUP);
  pinMode(LEDpin, OUTPUT);
  if(LittleFS.begin()) Serial.println("LittleFS OK");
  WiFi.begin(ssid, password);       // initialise and connect Wi-Fi
  while (WiFi.status() != WL_CONNECTED) delay(500);
  Serial.print("IP address ");Serial.println(WiFi.localIP());
  str = "<p><i>Image <span style=\"color:#009900;";
  baseHTML = str + "font-size:20px;\">attached</span> in email</i></p>";
  smtp.callback(sendCallback);      // callback funct on session close
  session.server.host_name = emailHost;
  session.server.port = emailPort;
  session.login.email = emailSender;
  session.login.password = emailPassword;
  message.sender.name = emailName;
  message.sender.email = emailSender;   // repeat sender email 
  message.subject = emailSubject;
  message.addRecipient(emailRcptName, emailRecipient);  // recipient email
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_normal;

  message.text.charSet = "us-ascii";    // instructions for text message
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  message.html.charSet = "utf-8";   // instructions for HTML code message
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

  att.descr.filename = "cap.jpg";      // name of image
  att.descr.mime = "image/jpg";     // JPEG formatted image
  att.file.path = "/cap.jpg";       // file location in SPIFFS
  att.file.storage_type = esp_mail_file_storage_type_flash;
  att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
//  message.addInlineImage(att);      // attach image (SPIFFS) to email 
  message.addAttachment(att);
  
  message.resetAttachItem(att);     // clear attached item data
  att.descr.filename = "logo.jpg";
  att.descr.mime = "image/jpg";
  att.blob.data = imageData;      // image data array in PROGMEM
  att.blob.size = sizeof(imageData);    // size of image data array
  att.descr.transfer_encoding = Content_Transfer_Encoding::enc_base64;
//  message.addParallelAttachment(att); // attach image (data array) to email
  message.addAttachment(att);
  
  if (!smtp.connect(&session)) return;    // connect email session
}

void loop()
{
  if(digitalRead(switchPin) == LOW)     // when switch pressed
  {
    digitalWrite(LEDpin, HIGH);     // turn on LED
    counter++;            // increment counter
//  newStr = baseStr + String(counter);   // text message commented out
//  message.text.content = newStr.c_str();
    newHTML = baseHTML + "<p>counter value: " + String(counter) + "</p>";
    message.html.content = newHTML.c_str(); // HTML code message
    message.clearHeader();        // reset email header
    header = "Message-ID: <" + String(millis()/1000.0) + "@gmail.com>";
    message.addHeader(header);
    if (!MailClient.sendMail(&smtp, &message, true))  // false: keep session open
        Serial.println("Error sending email, " + smtp.errorReason());
    digitalWrite(LEDpin, LOW);
  }
}

void sendCallback(SMTP_Status status)   // function for email send status
{
  Serial.println(status.info());        // print current status
  if (status.success()) Serial.println("email OK"); // email sent success
}
