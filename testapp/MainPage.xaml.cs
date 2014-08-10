using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using testapp.Resources;
using libtest;
using System.Threading;
using System.Net.Sockets;
using System.Threading.Tasks;
using Microsoft.Phone.Net.NetworkInformation;
namespace testapp
{
    
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            InitializeComponent();
            libtest.WindowsPhoneRuntimeComponent comp=new libtest.WindowsPhoneRuntimeComponent();
            comp.startService();
            //Thread trh = new Thread(comp.TestLibEvent);
            //trh.Start();
            //comp.TestLibEvent();
           
            // Sample code to localize the ApplicationBar
            //BuildLocalizedApplicationBar();
        }

        private  void Button_Click(object sender, RoutedEventArgs e)
        {
            SocketAsyncEventArgs args=new SocketAsyncEventArgs();
            args.RemoteEndPoint=new IPEndPoint(IPAddress.Parse("127.0.0.1"),5555);
            Socket sock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            args.Completed += args_Completed;
            sock.ConnectAsync(args);
           //DeviceNetworkInformation.
            
        }

        void recv_Completed(object sender, SocketAsyncEventArgs e)
        { 
            if(e.BytesTransferred<=0)
                return;
            byte[] szBuffer=new byte[e.BytesTransferred];
            Array.Copy(e.Buffer,szBuffer,e.BytesTransferred);
            string str = System.Text.UTF8Encoding.UTF8.GetString(szBuffer, 0, e.BytesTransferred);
        }

        void args_Completed(object sender, SocketAsyncEventArgs e)
        {
            if (e.ConnectSocket.Connected)
            {
                string strSent="helloworld";
                byte[] szSent=System.Text.UTF8Encoding.UTF8.GetBytes(strSent);
                SocketAsyncEventArgs args=new SocketAsyncEventArgs();
                args.SetBuffer(szSent,0,szSent.Length);
                e.ConnectSocket.SendAsync(args);

                SocketAsyncEventArgs argsRecv=new SocketAsyncEventArgs();
                argsRecv.SetBuffer(new byte[4096],0,4096);
                argsRecv.Completed += recv_Completed;
                e.ConnectSocket.ReceiveAsync(argsRecv);
            }
        }

        
        // Sample code for building a localized ApplicationBar
        //private void BuildLocalizedApplicationBar()
        //{
        //    // Set the page's ApplicationBar to a new instance of ApplicationBar.
        //    ApplicationBar = new ApplicationBar();

        //    // Create a new button and set the text value to the localized string from AppResources.
        //    ApplicationBarIconButton appBarButton = new ApplicationBarIconButton(new Uri("/Assets/AppBar/appbar.add.rest.png", UriKind.Relative));
        //    appBarButton.Text = AppResources.AppBarButtonText;
        //    ApplicationBar.Buttons.Add(appBarButton);

        //    // Create a new menu item with the localized string from AppResources.
        //    ApplicationBarMenuItem appBarMenuItem = new ApplicationBarMenuItem(AppResources.AppBarMenuItemText);
        //    ApplicationBar.MenuItems.Add(appBarMenuItem);
        //}
    }
}