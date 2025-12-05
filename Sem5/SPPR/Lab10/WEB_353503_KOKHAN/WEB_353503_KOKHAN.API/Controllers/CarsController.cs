//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Runtime.Intrinsics.Arm;
//using System.Threading.Tasks;
//using Microsoft.AspNetCore.Mvc;
//using Microsoft.AspNetCore.Mvc.Rendering;
//using Microsoft.EntityFrameworkCore;
//using WEB_353503_KOKHAN.API.Data;
//using WEB_353503_KOKHAN.API.Services.CarService;
//using WEB_353503_KOKHAN.Domain.Entities;
//using WEB_353503_KOKHAN.Domain.Models;

//namespace WEB_353503_KOKHAN.API.Controllers
//{
//    [Route("api/[controller]")]
//    [ApiController]
//    public class CarsController : ControllerBase
//    {
//        private readonly ICarService _carService;

//        public CarsController(ICarService carService)
//        {
//            _carService = carService;
//        }

//        [HttpGet("categories/{categoryName?}")]
//        public async Task<ActionResult<ResponseData<List<Car>>>> GetCars(string? categoryName, int pageNo = 1, int pageSize = 3)
//        {
//            var result = await _carService.GetProductListAsync(categoryName, pageNo, pageSize);
//            return Ok(result);
//        }

//        [HttpGet("{id}")]
//        public async Task<ActionResult<ResponseData<Car>>> GetCar(int id)
//        {
//            var result = await _carService.GetProductByIdAsync(id);
//            if (!result.Successfull || result.Data == null)
//            {
//                return NotFound(result.ErrorMessage);
//            }
//            return Ok(result);
//        }

//        [HttpPut("{id}")]
//        public async Task<IActionResult> PutCar(int id, Car car)
//        {
//            if (id != car.Id)
//            {
//                return BadRequest("Car ID mismatch.");
//            }

//            await _carService.UpdateProductAsync(id, car);
//            return NoContent();
//        }

//        [HttpPost]
//        public async Task<ActionResult<ResponseData<Car>>> PostCar(Car car)
//        {
//            var result = await _carService.CreateProductAsync(car);
//            return CreatedAtAction(nameof(GetCar), new { id = result.Data.Id }, result);
//        }

//        [HttpDelete("{id}")]
//        public async Task<IActionResult> DeleteCar(int id)
//        {
//            await _carService.DeleteProductAsync(id);
//            return NoContent();
//        }
//    }
//}
