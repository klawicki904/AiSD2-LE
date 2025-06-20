using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Media;
using System.Net;
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
            ExecuteGenerator();
            this.Close();
        }

        private void NumberCheck(object sender, TextCompositionEventArgs e)
        {
            var controls = new Object[] { vertNBox, fieldNBox, breweryNBox, pubNBox, sectionNBox,densityList,twoWayCheckbox,minFlowIntervalBox,maxFlowIntervalBox};
            var labels = new Label[] { vertNLabel, fieldNLabel, breweryNLabel, pubNLabel, sectionNLabel, densityLabel,twoWayLabel,flowIntervalLabel,flowIntervalDashLabel};
            GetControlIndex(ref sender, ref controls, out int controlIndex);
            e.Handled = !Regex.IsMatch(e.Text, @"^\d+$");
            if (!e.Handled && controlIndex!=controls.Length-1)
            {
                if (controlIndex == 4)
                {
                    (controls[5] as ComboBox).Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[5] as ComboBox).Foreground = Brushes.AntiqueWhite;
                    (controls[5] as ComboBox).SelectedIndex = 0;
                    (controls[5] as ComboBox).IsEnabled = true;
                    densityBackground.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[6] as CheckBox).Visibility = Visibility.Visible;
                    labels[6].Foreground= Brushes.AntiqueWhite;
                    twoWayBackground.Background= new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[7] as TextBox).Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[7] as TextBox).IsEnabled = true;
                    (controls[8] as TextBox).Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
                    (controls[8] as TextBox).IsEnabled = true;
                    labels[7].Foreground = Brushes.AntiqueWhite;
                    labels[8].Foreground = Brushes.AntiqueWhite;
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
            if (e.Key == Key.Space)
            {
                e.Handled = true;
                return;
            }
            if (!int.TryParse((sender as TextBox).Text, out int content)) return;
            if (content == 0 && (e.Key == Key.D0 || e.Key == Key.NumPad0)) e.Handled=true;
        }

        private void PasteCheck(object sender, DataObjectPastingEventArgs e)
        {
            e.CancelCommand();
        }

        private void InputDataCheck(object sender, TextChangedEventArgs e)
        {
            var controls = new Object[] { vertNBox, fieldNBox, breweryNBox, pubNBox, sectionNBox, densityList,twoWayCheckbox, minFlowIntervalBox, maxFlowIntervalBox };
            var labels = new Label[] { vertNLabel, fieldNLabel, breweryNLabel, pubNLabel, sectionNLabel, densityLabel,twoWayLabel, flowIntervalLabel, flowIntervalDashLabel };
            GetControlIndex(ref sender, ref controls, out int controlIndex);
            if ((sender as TextBox).Text.Length == 0)
            {
                for (int i = controlIndex + 1; i < controls.Length-1; i++)
                {
                    if (i == 5)
                    {
                        (controls[i] as ComboBox).Background = Brushes.Transparent;
                        (controls[i] as ComboBox).Foreground = Brushes.Transparent;
                        (controls[i] as ComboBox).IsEnabled = false;
                        densityBackground.Background = Brushes.Transparent;
                    }
                    else if (i == 6)
                    {
                        (controls[i] as CheckBox).Visibility = Visibility.Hidden;
                        twoWayBackground.Background = Brushes.Transparent;
                    }
                    else
                    {
                        (controls[i] as TextBox).Text = "";
                        (controls[i] as TextBox).Background = Brushes.Transparent;
                        (controls[i] as TextBox).IsEnabled = false;
                        if (i == 7)
                        {
                            (controls[i+1] as TextBox).Text = "";
                            (controls[i+1] as TextBox).Background = Brushes.Transparent;
                            (controls[i+1] as TextBox).IsEnabled = false;
                            labels[i + 1].Foreground = Brushes.Transparent;
                        }
                    }
                    labels[i].Foreground = Brushes.Transparent;
                }
            }
            if (RatioCheck()  && BreweryNCheck() && FlowIntervalCheck())
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
            if (controlIndex == 4)
            {
                if (!int.TryParse(vertNBox.Text, out int vertN)) return;
                if(!int.TryParse(fieldNBox.Text, out int fieldN))return;
                if(!int.TryParse(pubNBox.Text, out int pubN))return;
                double recommendedValue = Math.Round(0.16 * Math.Pow(vertN-2, 0.8));
                if (!RatioCheck()) generalInfoLabel.Content = "Rekomendowana wartość = " + recommendedValue + "\nPodano zbyt mało wierzchołków\nlub zbyt dużo zestawów wierzchołków!";
                else generalInfoLabel.Content = "Rekomendowana wartość = " + recommendedValue;
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
            var controls = new Object[] { vertNBox, fieldNBox, breweryNBox, pubNBox, sectionNBox, densityList,minFlowIntervalBox,maxFlowIntervalBox};
            GetControlIndex(ref sender, ref controls, out int controlIndex);
            switch (controlIndex) {
                case 0:
                    generalInfoLabel.Content = "Podaj odpowiednio dużą liczbę wierzchołków";
                    break;
                case 1:
                    generalInfoLabel.Content = "Rekomendowana wartość = " + Math.Round(int.Parse(vertNBox.Text)*0.12);
                    break;
                case 2:
                    generalInfoLabel.Content = "Rekomendowana wartość = " + Math.Round(int.Parse(vertNBox.Text) * 0.24, 0);
                    break;
                case 3:
                    generalInfoLabel.Content = "Rekomendowana wartość = " + Math.Round(int.Parse(vertNBox.Text) * 0.14);
                    break;
                case 4:
                    if (!int.TryParse(vertNBox.Text, out int vertN)) return;
                    if (!int.TryParse(fieldNBox.Text, out int fieldN)) return;
                    if (!int.TryParse(pubNBox.Text, out int pubN)) return;
                    double recommendedValue = Math.Round(0.16*Math.Pow(vertN-2,0.8));
                    if (!BreweryNCheck()) generalInfoLabel.Content = "Brak miejsca dla browarów!";
                    else if (!RatioCheck()) generalInfoLabel.Content = "Rekomendowana wartość = "+recommendedValue+"\nPodano zbyt mało wierzchołków\nlub zbyt dużo zestawów wierzchołków!";
                    else generalInfoLabel.Content = "Rekomendowana wartość = " + recommendedValue;
                    break;
                case 5:
                    generalInfoLabel.Content = "Różnorodność połączeń = Drogi skróty";
                    break;
                case 6:
                    generalInfoLabel.Content = "Minimalna wartość losowanej przepustowości";
                    break;
                case 7:
                    generalInfoLabel.Content = "Maksymalna wartość losowanej przepustowości";
                    break;

            }
        }
        private bool RatioCheck()
        {
            if (!int.TryParse(vertNBox.Text, out int vertN)) return false;
            if (!int.TryParse(sectionNBox.Text, out int sectionN)) return false;
            if (!int.TryParse(fieldNBox.Text, out int fieldN)) return false;
            if (!int.TryParse(pubNBox.Text, out int pubN)) return false;
            if (vertN < 8) return false;
            return (vertN-fieldN-pubN-7)/3+1 >= sectionN;
        }
        private bool BreweryNCheck()
        {
            if (!int.TryParse(vertNBox.Text, out int vertN)) return false;
            if (!int.TryParse(breweryNBox.Text, out int breweryN)) return false;
            if (!int.TryParse(fieldNBox.Text, out int fieldN)) return false;
            if (!int.TryParse(pubNBox.Text, out int pubN)) return false;
            return breweryN<=vertN-2-fieldN-pubN;
        }
        private bool FlowIntervalCheck()
        {
            if (!int.TryParse(minFlowIntervalBox.Text, out int minFlow)) return false;
            if (!int.TryParse(maxFlowIntervalBox.Text, out int maxFlow)) return false;
            if (minFlow < 0) return false;
            if (maxFlow < 1 || maxFlow < minFlow) return false;
            return true;
        }
        private void ExecuteGenerator()
        {
            string dataPath = "./generated_map.txt";
            string[] commandLineArgs = Environment.GetCommandLineArgs();
            if (commandLineArgs.Length == 2)
            {
                dataPath = commandLineArgs[1];
            }
            string isTwoWay = twoWayCheckbox.IsChecked.GetValueOrDefault() ? "1" : "0";
            string arguments = "\""+dataPath + "\" " + int.Parse(vertNBox.Text)+" "+int.Parse(sectionNBox.Text)+" "+densityList.SelectedIndex+" "
                +int.Parse(fieldNBox.Text)+" "+int.Parse(breweryNBox.Text)+" "+int.Parse(pubNBox.Text)+" "+isTwoWay+" "+int.Parse(minFlowIntervalBox.Text)+" "+int.Parse(maxFlowIntervalBox.Text);
            var generatorProcess = new Process
            {
                StartInfo = new ProcessStartInfo
                {
                    FileName = "Resources\\Generator\\networkGen.exe",
                    Arguments = arguments,
                    RedirectStandardOutput = false,
                    UseShellExecute = false,
                    CreateNoWindow = true
                }
            };
            generatorProcess.Start();
            /*
            if (doWriteDebugData)
            {
                using (StreamWriter debugLogFile = new StreamWriter("./gen_debug_log.txt"))
                {
                    debugLogFile.Write("networkGen debug log:");
                    while (!generatorProcess.StandardOutput.EndOfStream)
                    {
                        Console.WriteLine("linia");
                        debugLogFile.WriteLine(generatorProcess.StandardOutput.ReadLine());
                    }
                }
            }
            */
            generatorProcess.WaitForExit();
        }

        private void FillRecommendedData(object sender, RoutedEventArgs e)
        {
            if (!int.TryParse(vertNBox.Text, out int vertN))
            {
                generalInfoLabel.Content = "Najpierw podaj liczbę wierzchołków";
                return;
            }
            if (vertN < 8)
            {
                generalInfoLabel.Content = "Podano za mało wierzchołków";
                return;
            }
            fieldNBox.Text = Math.Round(int.Parse(vertNBox.Text) * 0.12).ToString();
            breweryNBox.Text = Math.Round(int.Parse(vertNBox.Text) * 0.24).ToString();
            pubNBox.Text = Math.Round(int.Parse(vertNBox.Text) * 0.14).ToString();
            int fieldN = int.Parse(fieldNBox.Text);
            int pubN = int.Parse(pubNBox.Text);
            sectionNBox.Text = Math.Round(0.16 * Math.Pow(vertN-2, 0.8)).ToString();
            densityList.SelectedIndex = 1;
            minFlowIntervalBox.Text = "15";
            maxFlowIntervalBox.Text = "45";

            breweryNBox.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
            pubNBox.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
            sectionNBox.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
            densityBackground.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
            densityList.Foreground = Brushes.AntiqueWhite;
            breweryNLabel.Foreground = Brushes.AntiqueWhite;
            pubNLabel.Foreground = Brushes.AntiqueWhite;
            sectionNLabel.Foreground = Brushes.AntiqueWhite;
            densityLabel.Foreground = Brushes.AntiqueWhite;
            breweryNBox.IsEnabled = true;
            pubNBox.IsEnabled = true;
            sectionNBox.IsEnabled = true;
            densityList.IsEnabled = true;
            twoWayCheckbox.Visibility = Visibility.Visible;
            twoWayLabel.Foreground = Brushes.AntiqueWhite;
            twoWayBackground.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
            minFlowIntervalBox.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
            maxFlowIntervalBox.Background = new SolidColorBrush(Color.FromRgb(68, 26, 112));
            flowIntervalLabel.Foreground = Brushes.AntiqueWhite;
            flowIntervalDashLabel.Foreground = Brushes.AntiqueWhite;
            minFlowIntervalBox.IsEnabled = true;
            maxFlowIntervalBox.IsEnabled = true;
        }
    }
}
