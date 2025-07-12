#include "viewADSHistory.h"
#include "ADS1115_controller.h"

String viewADSHistory::html = "";

String viewADSHistory::generateHTML()
{
    html = viewGeneric::basicHeader("ADS History");
    html += "<h1>ADS History</h1>";
    html += "<div class='scrollable-container' style='overflow-y: auto; max-height: 80vh;'>";
    html += "<table class='history-table' border='1'>";
    html += "<thead>";
    html += "<tr>";
    html += "<th>#</th>";
    html += "<th>Value</th>";
    html += "<th>Actions</th>";
    html += "</tr>";
    html += "</thead>";
    
    html += "<tbody id='ads-history-content'>";
    html += viewADSHistory::adsContent(); 
    html += "</tbody>";
    html += "</table>";
    html += "</div>";

    // Add script to update content every few seconds
    html += "<script>";
    html += "document.addEventListener('DOMContentLoaded', () => {";
    html += "  setInterval(() => {";
    html += "    fetch('/getADSValuesHistory')";
    html += "      .then(response => response.text())";
    html += "      .then(data => {";
    html += "        document.getElementById('ads-history-content').innerHTML = data;";
    html += "      });";
    html += "  }, 3000);"; 
    html += "});";
    html += "</script>";

    html += viewGeneric::defaultFooter();

    return html;
}

String viewADSHistory::adsContent()
{
    String content = "";
    ADS1115_controller* adsCtrl = ADS1115_controller::getInstance();
    const std::vector<float>& values = adsCtrl->getRecordedValues();

    if (values.empty()) {
        content += "<tr><td colspan='3'>No recorded values</td></tr>";
    } else {
        for (size_t i = 0; i < values.size(); i++) {
            content += "<tr>";
            content += "<td>" + String(i) + "</td>";
            content += "<td>" + String(values[i]) + "</td>";
            content += "<td>";
            content += "<a href='/deleteADSValue?index=" + String(i) + "' class='action-link delete-link'>Delete</a>";
            content += "</td>";
            content += "</tr>";
        }
    }
    
    return content;
}
