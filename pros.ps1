# File paths
$variablesFile = ".\src\Variable\variables.cpp"
$projectFile = ".\project.pros"

# Project names based on auton slot
$projectNames = @{
    1 = "53F Blue GoalRush";
    2 = "53F Blue Left";
    3 = "53F Blue Right";
    4 = "53F Red GoalRush";
    5 = "53F Red Left";
    6 = "53F Red Right";
    7 = "53F Skills";
    8 = "53F WS RedLeft";
    9 = "53F WSP RedLeft";
}

# Predefined auton lists for specific keywords
$autonKeywords = @{
    "red" = @(4, 5, 6, 8, 9, 7);  # Autons for "red"
    "blue" = @(1, 2, 3, 7);   # Autons for "blue"
    "both" = @(1, 2, 3, 4, 5, 6, 9, 7);
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

# Main logic to detect input type and process arguments
if ($args.Count -eq 0) {
    Write-Host "Error: No input provided. Please enter a number or keyword." -ForegroundColor Red
    exit 1
}

# Parse arguments
$input = $args[0]
$specifiedSlot = $args -contains "--slot" ? ($args[$args.IndexOf("--slot") + 1] -as [int]) : $null

# Check if input is a valid number
if ($input -match '^\d+$') {
    $Auton = [int]$input

    # Validate the auton number
    if ($Auton -lt 1 -or $Auton -gt $maxSlot) {
        Write-Host "Error: Invalid auton number '$Auton'. Please enter a valid auton number (1-$maxSlot)." -ForegroundColor Red
        exit 1
    }

    # Determine upload slot
    $uploadSlot = $specifiedSlot -ne $null ? $specifiedSlot : $Auton

    # Default behavior for a single auton slot
    if ($uploadSlot -lt 1 -or $uploadSlot -gt 8) {
        Write-Host "Error: Upload slot must be between 1 and 8." -ForegroundColor Red
        exit 1
    }

    Upload-Code -SlotNumber $uploadSlot -AutonValue $Auton -ProjectName $projectNames[$Auton]

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
