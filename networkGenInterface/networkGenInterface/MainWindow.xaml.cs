using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Automation.Peers;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using static System.Net.Mime.MediaTypeNames;

namespace networkGenInterface
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        private void AcceptUserInput(object sender, RoutedEventArgs e)
        {
            generalInfoLabel.Content = "Generowanie... Proszę czekać";
            ExecuteGenerator();
            this.Close();
        }

        private void NumberCheck(object sender, TextCompositionEventArgs e)
        {
            var controls = new Object[] { vertNBox, sectionNBox,densityList,fieldNBox,breweryNBox,pubNBox};
            var labels = new Label[] { vertNLabel, sectionNLabel, densityLabel,fieldNLabel,breweryNLabel,pubNLabel};
            GetControlIndex(ref sender, ref controls, out int controlIndex);
            e.Handled = !Regex.IsMatch(e.Text, @"^\d+$");
            if (!e.Handled && controlIndex!=controls.Length-1)
            {
                if (controlIndex == 1)
                {
                    (controls[2] as ComboBox).Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[2] as ComboBox).Foreground = Brushes.AntiqueWhite;
                    (controls[2] as ComboBox).SelectedIndex = 0;
                    (controls[2] as ComboBox).IsEnabled = true;
                    (controls[3] as TextBox).Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[3] as TextBox).IsEnabled = true;
                    labels[3].Foreground = Brushes.AntiqueWhite;
                    densityBackground.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                }
                else
                {
                    (controls[controlIndex + 1] as TextBox).Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[controlIndex + 1] as TextBox).IsEnabled = true;
                }
                labels[controlIndex+1].Foreground = Brushes.AntiqueWhite;
            }
        }

        private void KeyCheck(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Space) e.Handled = true;
        }

        private void PasteCheck(object sender, DataObjectPastingEventArgs e)
        {
            e.CancelCommand();
        }

        private void InputDataCheck(object sender, TextChangedEventArgs e)
        {
            var controls = new Object[] { vertNBox, sectionNBox, densityList, fieldNBox, breweryNBox, pubNBox };
            var labels = new Label[] { vertNLabel, sectionNLabel, densityLabel, fieldNLabel, breweryNLabel, pubNLabel};
            GetControlIndex(ref sender, ref controls, out int controlIndex);
            if ((sender as TextBox).Text.Length == 0)
            {
                for (int i = controlIndex + 1; i < controls.Length; i++)
                {
                    if (i == 2)
                    {
                        (controls[i] as ComboBox).Background = Brushes.Transparent;
                        (controls[i] as ComboBox).Foreground = Brushes.Transparent;
                        (controls[i] as ComboBox).IsEnabled = false;
                        densityBackground.Background = Brushes.Transparent;
                    }
                    else
                    {
                        (controls[i] as TextBox).Text = "";
                        (controls[i] as TextBox).Background = Brushes.Transparent;
                        (controls[i] as TextBox).IsEnabled = false;
                    }
                    labels[i].Foreground = Brushes.Transparent;
                }
            }
            if (VertSectionRatioCheck() && FieldPubNCheck() && BreweryNCheck())
            {
                generateButton.Visibility = Visibility.Visible;
                generateButton.IsEnabled = true;
                buttonLabel.Visibility = Visibility.Visible;
            }
            else
            {
                generateButton.Visibility = Visibility.Hidden;
                generateButton.IsEnabled = false;
                buttonLabel.Visibility = Visibility.Hidden;
            }
        }
        private void GetControlIndex(ref object sender, ref Object[] controls, out int controlIndex)
        {
            controlIndex = 0;
            for (int i = 1; i < controls.Length; i++)
            {
                if (sender == controls[i])
                {
                    controlIndex = i;
                    break;
                }
            }
        }
        private void ChangeGeneralInfo(object sender, RoutedEventArgs e)
        {
            var controls = new Object[] { vertNBox, sectionNBox, densityList, fieldNBox, breweryNBox, pubNBox };
            GetControlIndex(ref sender, ref controls, out int controlIndex);
            switch (controlIndex) {
                case 0:
                    generalInfoLabel.Content = "Podaj odpowiednio dużą liczbę wierzchołków";
                    break;
                case 1:
                    if ((int.Parse(vertNBox.Text)-2) / 5 - 1 < 0) generalInfoLabel.Content = "Rekomendowana wartość = 0";
                    else generalInfoLabel.Content = "Rekomendowana wartość = " + ((int.Parse(vertNBox.Text)-2) / 5 - 1).ToString();
                    break;
                case 2:
                    generalInfoLabel.Content = "Różnorodność połączeń = Drogi skróty";
                    break;
                case 3:
                    if (!VertSectionRatioCheck()) generalInfoLabel.Content = "!Niepoprawne poprzednie dane wejściowe!";
                    else generalInfoLabel.Content = "Rekomendowana wartość = " + (int)((int.Parse(vertNBox.Text) - 7 - (3.1 * int.Parse(sectionNBox.Text))) / 2);
                    break;
                case 4:
                    if (!VertSectionRatioCheck()) generalInfoLabel.Content = "!Niepoprawne poprzednie dane wejściowe!";
                    else generalInfoLabel.Content = "Rekomendowana wartość = " + Math.Round(int.Parse(vertNBox.Text) * 0.24, 0);
                    break;
                case 5:
                    if (!VertSectionRatioCheck()) generalInfoLabel.Content = "!Niepoprawne poprzednie dane wejściowe!";
                    else generalInfoLabel.Content = "Rekomendowana wartość = " + (int)((int.Parse(vertNBox.Text) - 7 - (3.1 * int.Parse(sectionNBox.Text))) / 2);
                    break;
            }
        }
        private bool VertSectionRatioCheck()
        {
            if (!int.TryParse(vertNBox.Text, out int vertN)) return false;
            if (!int.TryParse(sectionNBox.Text, out int sectionN)) return false;
            if (vertN < 10) return false;
            return (float)(vertN - 2) / (sectionN + 2) >= 2.6;
        }
        private bool FieldPubNCheck()
        {
            if (!int.TryParse(vertNBox.Text, out int vertN)) return false;
            if (!int.TryParse(sectionNBox.Text, out int sectionN)) return false;
            if (!int.TryParse(fieldNBox.Text, out int fieldN)) return false;
            if (!int.TryParse(pubNBox.Text, out int pubN)) return false;
            return (float)(vertN-2-fieldN-pubN) / (sectionN + 2) >= 3;
        }
        private bool BreweryNCheck()
        {
            if (!int.TryParse(vertNBox.Text, out int vertN)) return false;
            if (!int.TryParse(breweryNBox.Text, out int breweryN)) return false;
            if (!int.TryParse(fieldNBox.Text, out int fieldN)) return false;
            if (!int.TryParse(pubNBox.Text, out int pubN)) return false;
            return breweryN<=vertN-fieldN-pubN;
        }
        private void ExecuteGenerator()
        {
            string arguments = int.Parse(vertNBox.Text)+" "+int.Parse(sectionNBox.Text)+" "+densityList.SelectedIndex+" "
                +int.Parse(fieldNBox.Text)+" "+int.Parse(breweryNBox.Text)+" "+int.Parse(pubNBox.Text);

            var generatorProcess = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = "Resources\\Generator\\networkGen.exe",
                    Arguments = arguments,
                    RedirectStandardOutput = true,
                    UseShellExecute = false,
                    CreateNoWindow = true
                }
            };
            generatorProcess.Start();
            using (StreamWriter debugLogFile = new StreamWriter("networkGenDebugLog.txt"))
            {
                debugLogFile.Write("networkGen debug log:");
                while (!generatorProcess.StandardOutput.EndOfStream)
                {
                    debugLogFile.WriteLine(generatorProcess.StandardOutput.ReadLine());
                    
                }
            }
            generatorProcess.WaitForExit();
        }

    }
}
