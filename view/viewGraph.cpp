#include "viewGraph.h"
#include <set> 

String viewGraph::html = "";
String viewGraph::generateHTML()
{
    html = viewGeneric::defaultCssHeader("Graph View");

    html += "<h1>Graph View</h1>";

    
    std::vector<int> addresses = SystemState::getInstance()->getAllRegisterAddress();
    int i = 0;
    for (int addr : addresses)
    {
        addresses[i] += 1000; //operazione di shift
    }
    
    String apiFetch = "getRegisterValues";
    String apiFetchParam = "address";

    html += generateGraph(addresses, apiFetch, apiFetchParam);

    html += viewGeneric::defaultFooter();

    return html;
}


String viewGraph::generateGraph(std::vector<int> addresses, String apiFetch, String apiFetchParam)
{
    String var_html = "";

    // Menu a tendina per selezionare l'indirizzo del registro
    var_html += "<label for='           register-select'>Select Register Address: or gpio value</label>";
    var_html += "<select id='register-select' onchange='updateGraph()'>";

    std::set<int> seen;

    for (int addr : addresses)
    {
        if (seen.find(addr) != seen.end()) {
            // Se l'indirizzo è già stato aggiunto, salto la generazione dell'option.
            continue;
        }
        seen.insert(addr);

        if (addr > 0 && addr < 1000) {
            var_html += "<option value='" + String(addr) + "'>" + String(addr) + "</option>";
        } else if (addr <= 0) 
        {
            var_html += "<option value='" + String(addr) + "'>Multiplex:" + String(-addr) + "</option>";
        }
        else if (addr >= 1000 && addr < 2000) {
            var_html += "<option value='" + String(addr - 1000) + "'>ModbusValue:" + String(addr - 1000) + "</option>";
        
        } else {
            var_html += "<option value='" + String(addr) + "'>Errore inserimento!" + String(addr - 3000) + "</option>";
        }
    }


    var_html += "</select>";

    // Contenitore per il grafico
    var_html += "<canvas id='myChart' width='400' height='200'></canvas>";

    // Script JavaScript per Chart.js e per gestire il grafico
    var_html += "<script src='https://cdn.jsdelivr.net/npm/chart.js' async></script>";
    var_html += "<script>";
    var_html += "let chart;";
    var_html += "function updateGraph() {";
    var_html += "  const address = document.getElementById('register-select').value;";
    var_html += "  fetch('/"+apiFetch+"?"+apiFetchParam+"=' + address)";
    var_html += "    .then(response => response.json())";
    var_html += "    .then(data => {";
    var_html += "      const labels = data.map((_, index) => index + 1);";
    var_html += "      const values = data;";
    var_html += "      const ctx = document.getElementById('myChart').getContext('2d');";
    var_html += "      if (chart) chart.destroy();";
    var_html += "      chart = new Chart(ctx, {";
    var_html += "        type: 'line',";
    var_html += "        data: {";
    var_html += "          labels: labels,";
    var_html += "          datasets: [{";
    var_html += "            label: 'Register Values',";
    var_html += "            data: values,";
    var_html += "            borderColor: 'rgba(75, 192, 192, 1)',";
    var_html += "            borderWidth: 2";
    var_html += "          }]";
    var_html += "        },";
    var_html += "        options: {";
    var_html += "          responsive: true,";
    var_html += "          animation: false,"; // Disabilita l'animazione
    var_html += "          scales: {";
    var_html += "            x: {";
    var_html += "              display: true,";
    var_html += "              title: {";
    var_html += "                display: true,";
    var_html += "                text: 'Sample Index'";
    var_html += "              }";
    var_html += "            },";
    var_html += "            y: {";
    var_html += "              display: true,";
    var_html += "              title: {";
    var_html += "                display: true,";
    var_html += "                text: 'Value'";
    var_html += "              }";
    var_html += "            }";
    var_html += "          }";
    var_html += "        }";
    var_html += "      });";
    var_html += "    });";
    var_html += "}";
    var_html += "document.addEventListener('DOMContentLoaded', () => {";
    var_html += "  updateGraph();";                  // Per caricare il grafico all'avvio
    var_html += "  setInterval(updateGraph, 1000);"; // Aggiorna il grafico ogni 5 secondi
    var_html += "});";
    var_html += "</script>";

    return var_html;
}

String viewGraph::initCirularProgressBarGraph()
{
    String var_circle_progressbar_html = "";
    var_circle_progressbar_html += "<script src='https://cdn.jsdelivr.net/npm/chart.js' async></script>";
    var_circle_progressbar_html += "<div class='circle_progressbar_chart-container' style='font-family: Raleway, sans-serif; display: flex; flex-wrap: wrap; justify-content: space-around; gap: 20px; padding: 20px;'>";
    var_circle_progressbar_html += "<script>var charts = {};</script>"; // Aggiungi questa linea per inizializzare l'oggetto charts
    return var_circle_progressbar_html;
}

String viewGraph::endCirularProgressBarGraph()
{
    String var_circle_progressbar_html = "";
    var_circle_progressbar_html += "</div>";
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "function createChart(ctx, percentage, chartId) {";
    var_circle_progressbar_html += "  if (charts[chartId]) {";
    var_circle_progressbar_html += "    charts[chartId].destroy();";
    var_circle_progressbar_html += "  }";
    var_circle_progressbar_html += "  charts[chartId] = new Chart(ctx, {";
    var_circle_progressbar_html += "    type: 'doughnut',";
    var_circle_progressbar_html += "    data: {";
    var_circle_progressbar_html += "      datasets: [{";
    var_circle_progressbar_html += "        data: [percentage, 100 - percentage],";
    var_circle_progressbar_html += "        backgroundColor: ['#4caf50', '#ddd'],";
    var_circle_progressbar_html += "        borderWidth: 0";
    var_circle_progressbar_html += "      }]";
    var_circle_progressbar_html += "    },";
    var_circle_progressbar_html += "    options: {";
    var_circle_progressbar_html += "      cutout: '80%',";
    var_circle_progressbar_html += "      animation: { duration: 0 },";
    var_circle_progressbar_html += "      plugins: {";
    var_circle_progressbar_html += "        tooltip: { enabled: false },";
    var_circle_progressbar_html += "        legend: { display: false }";
    var_circle_progressbar_html += "      }";
    var_circle_progressbar_html += "    }";
    var_circle_progressbar_html += "  });";
    var_circle_progressbar_html += "}";
    var_circle_progressbar_html += "function updateChart(apiUrl, ctx, labelId, chartId) {";
    var_circle_progressbar_html += "  fetch(apiUrl)";
    var_circle_progressbar_html += "    .then(response => response.json())";
    var_circle_progressbar_html += "    .then(data => {";
    var_circle_progressbar_html += "      const percentage = (data.used / data.total) * 100;";
    var_circle_progressbar_html += "      createChart(ctx, percentage, chartId);";
    var_circle_progressbar_html += "      document.getElementById(labelId).innerText = percentage.toFixed(2) + ' %';";
    var_circle_progressbar_html += "    })";
    var_circle_progressbar_html += "    .catch(error => console.error('Error fetching data:', error));";
    var_circle_progressbar_html += "}";
    var_circle_progressbar_html += "</script>";
    return var_circle_progressbar_html;
}

String viewGraph::generateCirularProgressBarGraph(String circle_progressbar_label, float circle_progressbar_used_value, float circle_progressbar_total_value, String apiFetchData, int timeToUpdate)
{
    String var_circle_progressbar_html = "";
    float percentage = (circle_progressbar_used_value / circle_progressbar_total_value) * 100;
    var_circle_progressbar_html += "<div class='circle_progressbar_chart-wrapper' style='display: flex; flex-direction: column; align-items: center; width: 45%; max-width: 300px; min-width: 150px; position: relative;'>";
    var_circle_progressbar_html += "    <div class='circle_progressbar_label' style='font-size: 1.2rem; margin-bottom: 10px;'> " + circle_progressbar_label + "</div>";
    var_circle_progressbar_html += "    <canvas id='circle_progressbar_chart_" + circle_progressbar_label + "' class='circle_progressbar_chart' style='width: 100%; height: auto;' width='200' height='200'></canvas>";
    var_circle_progressbar_html += "    <div class='circle_progressbar_center-label' id='label" + circle_progressbar_label + "' style='position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); font-size: 2rem; font-weight: bold; color: #000; text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);'>" + String((int)percentage) + " %</div>";
    var_circle_progressbar_html += "<div class='circle_progressbar_percentage' style='font-size: 1rem; margin-top: 10px;'>Utilizzato: " + String(circle_progressbar_used_value) + " / Totale: " + String(circle_progressbar_total_value) + "</div>";
    var_circle_progressbar_html += "</div>";
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "document.addEventListener('DOMContentLoaded', () => {";
    var_circle_progressbar_html += "  const ctx = document.getElementById('circle_progressbar_chart_" + circle_progressbar_label + "').getContext('2d');";
    var_circle_progressbar_html += "  const percentage = " + String(percentage) + ";";
    var_circle_progressbar_html += "  createChart(ctx, percentage, 'circle_progressbar_chart_" + circle_progressbar_label + "');";
    var_circle_progressbar_html += "  setInterval(() => {";
    var_circle_progressbar_html += "    updateChart('" + apiFetchData + "', ctx, 'label" + circle_progressbar_label + "', 'circle_progressbar_chart_" + circle_progressbar_label + "');";
    var_circle_progressbar_html += "  }, " + String(timeToUpdate) + ");";
    var_circle_progressbar_html += "});";
    var_circle_progressbar_html += "</script>";
    return var_circle_progressbar_html;
}

String viewGraph::generateCirularProgressBarGraph(String circle_progressbar_label, float circle_progressbar_used_value, float circle_progressbar_total_value)
{
    String var_circle_progressbar_html = "";
    float percentage = (circle_progressbar_used_value / circle_progressbar_total_value) * 100;
    var_circle_progressbar_html += "<div class='circle_progressbar_chart-wrapper' style='display: flex; flex-direction: column; align-items: center; width: 45%; max-width: 300px; min-width: 150px; position: relative;'>";
    var_circle_progressbar_html += "    <div class='circle_progressbar_label' style='font-size: 1.2rem; margin-bottom: 10px;'> " + circle_progressbar_label + "</div>";
    var_circle_progressbar_html += "    <canvas id='circle_progressbar_chart_" + circle_progressbar_label + "' class='circle_progressbar_chart' style='width: 100%; height: auto;' width='200' height='200'></canvas>";
    var_circle_progressbar_html += "    <div class='circle_progressbar_center-label' id='label" + circle_progressbar_label + "' style='position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); font-size: 2rem; font-weight: bold; color: #000; text-shadow: 1px 1px 2px rgba(0, 0, 0, 0.2);'>" + String((int)percentage) + " %</div>";
    var_circle_progressbar_html += "<div class='circle_progressbar_percentage' style='font-size: 1rem; margin-top: 10px;'>Utilizzato: " + String(circle_progressbar_used_value) + " / Totale: " + String(circle_progressbar_total_value) + "</div>";
    var_circle_progressbar_html += "</div>";
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "document.addEventListener('DOMContentLoaded', () => {";
    var_circle_progressbar_html += "  const ctx = document.getElementById('circle_progressbar_chart_" + circle_progressbar_label + "').getContext('2d');";
    var_circle_progressbar_html += "  const percentage = " + String(percentage) + ";";
    var_circle_progressbar_html += "  createChart(ctx, percentage);";
    var_circle_progressbar_html += "});";
    var_circle_progressbar_html += "</script>";

    return var_circle_progressbar_html;
}

