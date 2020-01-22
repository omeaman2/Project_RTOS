import xlsxwriter

workbook = xlsxwriter.Workbook('chart_line.xlsx')
worksheet = workbook.add_worksheet()
bold = workbook.add_format({'bold': 1})

# Add the worksheet data that the charts will refer to.
headings = ['Number', 'Batch 1', 'Batch 2']
data = [
        []
,
        []
]

datalength = len(data[0])
worksheet.write_row('A1', headings, bold)
worksheet.write_column('A2', data[0])
worksheet.write_column('B2', data[1])
cData = [];
for i in range(datalength):
	cData.append(data[0][i] - data[1][i]);
worksheet.write_column('C2', cData)

# Create a new chart object. In this case an embedded chart.
chart1 = workbook.add_chart({'type': 'line'})


# Configure the first series.
chart1.add_series({
    'values':     '=Sheet1!$A$2:$A$'+ str(datalength)
})
chart1.add_series({
	'values':     '=Sheet1!$B$2:$B$'+ str(datalength)
})
chart1.add_series({
	'values':     '=Sheet1!$C$2:$C$'+ str(datalength)
})


# Add a chart title and some axis labels.
chart1.set_title ({'name': 'Results of sample analysis'})
chart1.set_x_axis({'name': 'Test number'})
chart1.set_y_axis({'name': 'Sample length (mm)'})

# Set an Excel chart style. Colors with white outline and shadow.
chart1.set_style(10)

# Insert the chart into the worksheet (with an offset).
worksheet.insert_chart('D2', chart1, {'x_offset': 25, 'y_offset': 10})


workbook.close()
