import json


def create_system_row(system_data, html_file):
    html_file.write('<tr>\n')

    html_file.write('<td>' + system_data['name'] + '</td>\n')

    html_file.write('<td>\n')
    if 'componentsRead' in system_data:
        components_read = system_data['componentsRead']
        for component in components_read:
            html_file.write(component + '<br />\n')
    html_file.write('</td>\n')

    html_file.write('<td>\n')
    if 'componentsWrite' in system_data:
        components_write = system_data['componentsWrite']
        for component in components_write:
            html_file.write(component + '<br />\n')
    html_file.write('</td>\n')

    html_file.write('</tr>\n')


def convert_to_html(json_data):
    html_file = open('systemsDocs.md', 'w')
    html_file.write('<html>\n')
    html_file.write('<head>\n')
    html_file.write('<link rel="stylesheet" href="style.css">\n')
    html_file.write('</head>\n')
    html_file.write('<body>\n')
    html_file.write('<table>\n')
    html_file.write('<tr>\n')

    html_file.write('<th>System name</th>\n')
    html_file.write('<th>Components Read</th>\n')
    html_file.write('<th>Components Modified</th>\n')

    html_file.write('</tr>\n')

    for system in json_data:
        create_system_row(system, html_file)

    html_file.write('</table>\n')
    html_file.write('</body>\n')
    html_file.write('</html>\n')


with open('systemsData.json', 'r') as file:
    systemData = json.load(file)

systemData = systemData['systems']
convert_to_html(systemData)
