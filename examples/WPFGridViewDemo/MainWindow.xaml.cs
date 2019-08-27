using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace GridViewDemo
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        List<Info> items = new List<Info>();

        public MainWindow()
        {
            InitializeComponent();
            items.Add(new Info() { Name = "Item1", Value = "Value1" });
            items.Add(new Info() { Name = "Item2", Value = "Value2" });
            items.Add(new Info() { Name = "Item3", Value = "Value3" });
            lstView.ItemsSource = items;
        }

        public class Info
        {
            public string Name { get; set; }

            public string Value { get; set; }

        }

        private void btnUpdate_Click(object sender, RoutedEventArgs e)
        {
            items[0].Value = "Value5"; // modify existing value
            items.Add(new Info() { Name = "Item4", Value = "Value4" }); // add new value

            lstView.Items.Refresh(); // force repaint of control
        }
    }
}
