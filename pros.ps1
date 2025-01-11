# File paths
$variablesFile = ".\src\Variable\variables.cpp"
$projectFile = ".\project.pros"

# Project names based on auton slot
$projectNames = @{
    1 = "53F_HS Blue GoalRush";
    2 = "53F_HS Blue Left";
    3 = "53F_HS Blue Right";
    4 = "53F_HS Red GoalRush";
    5 = "53F_HS Red Left";
    6 = "53F_HS Red Right";
    7 = "53F_HS Skills";
    8 = "53F_HS WS RedLeft";
    9 = "53F_HS WSP RedLeft";
}

# Predefined auton lists for specific keywords
$autonKeywords = @{
    "red" = @(4, 5, 6, 8, 9, 7);  # Autons for "red"
    "blue" = @(1, 2, 3, 7);   # Autons for "blue"
}

# Determine the maximum slot number
$maxSlot = $projectNames.Keys | Measure-Object -Maximum | Select-Object -ExpandProperty Maximum

# Function to upload auton code
function Upload-Code {
    param (
        [int]$SlotNumber,    # Upload slot number
        [int]$AutonValue,    # Auton value
        [string]$ProjectName # Project name
    )

    # Cap the upload slot at 8
    $uploadSlot = if ($SlotNumber -gt 8) { 8 } else { $SlotNumber }

    # Update variables.cpp
    $content = Get-Content $variablesFile
    # Search and replace the line with 'auton ='
    $content = $content | ForEach-Object {
        if ($_ -match 'int auton = ') {
            "int auton = $AutonValue;"  # Replace the line with the updated auton value
        } else {
            $_  # Leave other lines as they are
        }
    }
    $content | Set-Content $variablesFile
    Write-Host "Updated variables.cpp: Set auton to $AutonValue"

    # Update project.pros
    $projectContent = Get-Content $projectFile
    $projectContent = $projectContent -replace '"project_name": ".*",', "`"project_name"": ""$ProjectName`","
    $projectContent = $projectContent -replace 'slot = \d+', "slot = $uploadSlot"
    $projectContent | Set-Content $projectFile
    Write-Host "Updated project.pros: Set slot to $uploadSlot and project_name to '$ProjectName'"

    # Upload code to the capped slot
    Write-Host "Uploading code to slot $uploadSlot..."
    pros mu --slot $uploadSlot
}

# Main logic to detect input type
if ($args.Count -eq 0) {
    Write-Host "Error: No input provided. Please enter a number or keyword." -ForegroundColor Red
    exit 1
}

$input = $args[0]

# Check if input is a valid number
if ($input -match '^\d+$') {
    $Slot = [int]$input

    # Validate the auton slot
    if ($Slot -lt 1 -or $Slot -gt $maxSlot) {
        Write-Host "Error: Invalid auton number '$Slot'. Please enter a valid auton number (1-$maxSlot)." -ForegroundColor Red
        exit 1
    }

    # Default behavior for a single slot
    Upload-Code -SlotNumber $Slot -AutonValue $Slot -ProjectName $projectNames[$Slot]
} elseif ($autonKeywords.ContainsKey($input)) {
    # If input is a keyword, get the corresponding auton list
    $autonList = $autonKeywords[$input]
    $uploadSlot = 1

    # Upload each auton in the list
    foreach ($auton in $autonList) {
        if ($projectNames.ContainsKey($auton)) {
            Upload-Code -SlotNumber $uploadSlot -AutonValue $auton -ProjectName $projectNames[$auton]
            $uploadSlot++
        }
    }

    # Fill remaining slots with empty values
    while ($uploadSlot -le 8) {
        Upload-Code -SlotNumber $uploadSlot -AutonValue -1 -ProjectName "Empty Slot"
        $uploadSlot++
    }
} else {
    Write-Host "Error: Invalid input '$input'. Please enter a valid number or keyword." -ForegroundColor Red
    exit 1
}

Write-Host "Done!"
